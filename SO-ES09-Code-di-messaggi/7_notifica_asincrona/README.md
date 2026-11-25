# Notifica asincrona: ``mq_notify``

Con ``mq_notify`` un processo chiede al sistema di essere **avvisato** quando arriva un messaggio in una coda vuota, invece di restare fermo in una ``mq_receive``. In questo esempio l'avviso è l'esecuzione di una funzione in un nuovo thread (``SIGEV_THREAD``), mentre il resto del programma continua a lavorare.

## Quando serve

Un processo deve ricevere messaggi da una coda, ma nel frattempo ha altro da fare. Con le primitive viste finora le strade sono queste:

| Soluzione | Cosa fa il processo finché la coda è vuota | Limite |
|---|---|---|
| ``mq_receive`` bloccante | resta sospeso | non può fare altro |
| ``O_NONBLOCK`` e ``mq_receive`` ripetuta (*polling*) | riprova a intervalli | se riprova spesso spreca CPU, se riprova di rado risponde in ritardo |
| ``mq_timedreceive`` | resta sospeso al massimo fino a una scadenza | è comunque un'attesa, solo limitata |
| un thread dedicato con ``mq_receive`` bloccante | il thread resta sospeso, gli altri lavorano | un thread occupato solo ad aspettare |
| ``mq_notify`` | niente: è il sistema ad avvisarlo quando arriva un messaggio | va usata rispettando le regole qui sotto |

Il thread dedicato è spesso la soluzione più semplice. ``mq_notify`` lascia invece al sistema il compito di accorgersi dell'arrivo dei messaggi, e il programma se ne occupa solo quando serve.

## Come si usa

```c
#include <mqueue.h>
#include <signal.h>     /* costanti SIGEV_* */

int mq_notify(mqd_t mqdes, const struct sigevent *sevp);
```

Restituisce ``0``, oppure ``-1`` in caso di errore. La struttura ``sigevent`` descrive *come* essere avvisati; i campi usati nell'esempio sono:

```c
struct sigevent sev;
memset(&sev, 0, sizeof(sev));
sev.sigev_notify = SIGEV_THREAD;               /* avviso = una funzione eseguita in un nuovo thread */
sev.sigev_notify_function = gestore_notifica;  /* la funzione: void gestore_notifica(union sigval) */
sev.sigev_notify_attributes = NULL;            /* attributi del thread: quelli predefiniti */
sev.sigev_value.sival_ptr = &queue;            /* argomento che riceverà la funzione */
mq_notify(queue, &sev);
```

Il campo ``sigev_notify`` può valere:

- ``SIGEV_THREAD``: all'arrivo del messaggio viene eseguita ``sigev_notify_function`` in un thread nuovo, come se fosse la funzione passata a ``pthread_create``;
- ``SIGEV_SIGNAL``: il processo riceve il segnale indicato in ``sigev_signo`` (da gestire con un *handler*);
- ``SIGEV_NONE``: il processo risulta registrato, ma non riceve alcun avviso.

Con ``sevp == NULL`` la registrazione viene cancellata.

### Le regole

Sono quattro frasi di ``man 3 mq_notify``, e spiegano tutto il codice dell'esempio.

1. **Solo se la coda era vuota.** La notifica parte quando un messaggio arriva in una coda *vuota*. Se al momento della registrazione la coda contiene già dei messaggi, la notifica arriverà solo dopo che la coda è stata svuotata ed è arrivato un messaggio nuovo.
2. **Una volta sola.** Dopo una notifica la registrazione viene cancellata: per ricevere la successiva bisogna chiamare di nuovo ``mq_notify``, e conviene farlo *prima* di svuotare la coda. Per svuotarla senza bloccarsi quando è vuota, la coda va aperta con ``O_NONBLOCK``.
3. **Un solo processo per coda.** Se un altro processo è già registrato, ``mq_notify`` fallisce con ``errno == EBUSY``. La registrazione si cancella con ``mq_notify(q, NULL)``, chiudendo il descrittore con ``mq_close`` oppure alla terminazione del processo.
4. **Chi è già in attesa ha la precedenza.** Se un altro processo (o thread) è bloccato in ``mq_receive`` sulla coda vuota, il messaggio va a lui e la notifica non parte; la registrazione resta valida.

Dalle regole 1 e 2 viene lo schema di ogni gestore di notifica:

```text
gestore della notifica:
    ri-registrati con mq_notify
    ripeti mq_receive (non bloccante) finché restituisce -1 con errno == EAGAIN
```

Perché prima ri-registrarsi e poi svuotare? Se si svuotasse prima, un messaggio arrivato tra la fine dello svuotamento e la nuova registrazione resterebbe in coda senza far partire nessuna notifica; da quel momento la coda non sarebbe più vuota e, per la regola 1, nessuna notifica partirebbe più (è l'[esperimento 3](#esperimenti)). Ri-registrandosi prima, un messaggio che arriva durante lo svuotamento viene letto dal ciclo, e uno che arriva dopo trova la coda vuota e fa partire una nuova notifica.

## Il programma

Il file [notifica.c](notifica.c) crea un processo figlio che fa da mittente; il padre si registra per la notifica e poi "lavora" per 8 secondi, senza mai chiamare una ``mq_receive`` bloccante.

```text
 t (s)   mittente (figlio)          main (padre)                 thread di notifica
   0                                registrazione, poi lavoro
   2     invia "primo"                  ...                      riceve "primo"
   4     invia "secondo" e "terzo"      ...                      riceve "secondo" e "terzo"
   6     invia "quarto"                 ...                      riceve "quarto"
   8                                cancella la registrazione,
                                    stampa i messaggi rimasti
```

- **``main``:** crea la coda in sola lettura e con ``O_NONBLOCK`` (``O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK``), crea il figlio, chiama ``registra_notifica`` e poi esegue il ciclo di "lavoro". Alla fine attende il figlio, cancella la registrazione con ``mq_notify(queue, NULL)``, stampa quanti messaggi sono rimasti in coda (con ``mq_getattr``), chiude e rimuove la coda.
- **figlio (mittente):** apre la coda **per nome** in sola scrittura, senza ``O_NONBLOCK``, e invia i quattro messaggi con ``mq_send``. ``O_NONBLOCK`` è una proprietà del descrittore, non della coda: il descrittore del mittente è bloccante, quello del padre no.
- **``registra_notifica``:** riempie la ``struct sigevent`` e chiama ``mq_notify``.
- **``gestore_notifica``:** è la funzione eseguita a ogni notifica. Segue lo schema delle regole: ri-registrazione, poi ``mq_receive`` non bloccanti fino a ``EAGAIN``. Riceve come argomento il valore messo in ``sigev_value`` (qui l'indirizzo del descrittore della coda).

## Compilazione ed esecuzione

```console
$ make
$ ./start
[main, thread 180202] registrato con mq_notify, ora lavoro
[main] lavoro... (1/8)
[main] lavoro... (2/8)
[mittente] invio <primo>
[main] lavoro... (3/8)
   [notifica, thread 180239] la coda non e' piu' vuota
   [notifica, thread 180239] ricevuto <primo>
[main] lavoro... (4/8)
[mittente] invio <secondo>
[mittente] invio <terzo>
   [notifica, thread 180278] la coda non e' piu' vuota
   [notifica, thread 180278] ricevuto <secondo>
   [notifica, thread 180278] ricevuto <terzo>
[main] lavoro... (5/8)
[main] lavoro... (6/8)
[mittente] invio <quarto>
   [notifica, thread 180303] la coda non e' piu' vuota
   [notifica, thread 180303] ricevuto <quarto>
[main] lavoro... (7/8)
[main] lavoro... (8/8)
[main] messaggi rimasti in coda: 0
```

Si osservi che:

- il main non si ferma mai: le righe "lavoro" continuano ogni secondo, e i messaggi vengono ricevuti nel frattempo da un altro thread;
- ogni notifica è gestita da un thread **diverso** (il numero è il TID, l'identificativo del thread nel kernel, restituito da ``gettid()``; quello del main coincide con il PID del processo);
- "secondo" e "terzo", inviati uno subito dopo l'altro, arrivano con **una sola** notifica: quando arriva "terzo" la coda non è vuota (regola 1), ed è il ciclo di svuotamento a prenderli entrambi. Se il thread fosse così veloce da svuotare la coda prima dell'arrivo di "terzo", "terzo" farebbe partire una seconda notifica: anche questo sarebbe corretto;
- alla fine non resta nessun messaggio in coda.

> **_N.B.:_** ``gettid()`` richiede glibc 2.30 o successiva (Ubuntu 22.04 ha la 2.35) e la macro ``_GNU_SOURCE``, definita all'inizio del file.

## Osservare la registrazione

Mentre il programma è in esecuzione, da un altro terminale:

```console
$ cat /dev/mqueue/so_es08_notify
QSIZE:0          NOTIFY:2     SIGNO:0     NOTIFY_PID:171015
$ ps -L -o pid,lwp,comm,wchan:28 -p $(pgrep -o -x start)
    PID     LWP COMMAND         WCHAN
 171015  171015 start           hrtimer_nanosleep
 171015  171017 start           skb_wait_for_more_packets
```

- ``NOTIFY_PID`` è il processo registrato (il padre). ``NOTIFY`` è il tipo di avviso: ``0`` = ``SIGEV_SIGNAL``, ``1`` = ``SIGEV_NONE``, ``2`` = ``SIGEV_THREAD`` (``man 7 mq_overview``).
- ``ps -L`` elenca i thread del padre (``LWP`` è il TID). Oltre al main, fermo nella ``sleep``, c'è un secondo thread che il programma non ha mai creato: è il thread di servizio della libreria C (si veda il dettaglio più sotto).

## Esperimenti

Ogni esperimento cambia una sola riga di ``notifica.c``. Dopo ogni prova: ``make`` per ricompilare, e alla fine ripristinare la riga originale. Negli output sono omesse le righe "lavoro".

**1. Dimenticare di rinnovare la registrazione.** In ``gestore_notifica`` commentare ``registra_notifica(queue);``.

```text
[main, thread 180371] registrato con mq_notify, ora lavoro
[mittente] invio <primo>
   [notifica, thread 180404] la coda non e' piu' vuota
   [notifica, thread 180404] ricevuto <primo>
[mittente] invio <secondo>
[mittente] invio <terzo>
[mittente] invio <quarto>
[main] messaggi rimasti in coda: 3
```

La notifica vale una volta sola (regola 2): dopo "primo" nessuno è più registrato, e gli altri tre messaggi restano in coda.

**2. Una sola ``mq_receive`` per notifica.** Nel ciclo di ``gestore_notifica`` aggiungere ``break;`` subito dopo la ``printf`` di "ricevuto".

```text
[main, thread 170940] registrato con mq_notify, ora lavoro
[mittente] invio <primo>
   [notifica, thread 170963] la coda non e' piu' vuota
   [notifica, thread 170963] ricevuto <primo>
[mittente] invio <secondo>
[mittente] invio <terzo>
   [notifica, thread 170977] la coda non e' piu' vuota
   [notifica, thread 170977] ricevuto <secondo>
[mittente] invio <quarto>
[main] messaggi rimasti in coda: 2
```

"terzo" resta in coda, quindi la coda non torna mai vuota: quando arriva "quarto" non parte nessuna notifica (regola 1), anche se il processo è registrato. Per questo il gestore deve svuotare la coda.

**3. Registrarsi quando la coda non è vuota.** Nel ``main`` aggiungere ``sleep(3);`` subito prima di ``registra_notifica(&queue);``.

```text
[mittente] invio <primo>
[main, thread 180500] registrato con mq_notify, ora lavoro
[mittente] invio <secondo>
[mittente] invio <terzo>
[mittente] invio <quarto>
[main] messaggi rimasti in coda: 4
```

Al momento della registrazione "primo" è già in coda: nessuna notifica, né ora né dopo, perché la coda non si svuota mai (regola 1). Come si corregge? Dopo essersi registrati, si svuota subito la coda una volta, con lo stesso ciclo del gestore.

## Dettaglio: funzione C o system call?

Con ``SIGEV_THREAD`` buona parte del lavoro non la fa il kernel ma la libreria C, perché i thread che eseguono il gestore li crea e li gestisce la libreria dei pthread (``man 3 mq_notify``, sezione *C library/kernel differences*). Si vede con ``strace`` (righe accorciate):

```console
$ strace -f -e trace=socket,mq_notify,clone3,recvfrom ./start
...
[pid 171113] socket(AF_NETLINK, SOCK_RAW|SOCK_CLOEXEC, NETLINK_ROUTE) = 4
[pid 171113] clone3({flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|..., ...}) = 171115
[pid 171113] mq_notify(3, {sigev_value={...}, sigev_signo=SIGILL, sigev_notify=SIGEV_THREAD, ...}) = 0
[pid 171115] recvfrom(4, ..., 32, MSG_WAITALL|MSG_NOSIGNAL, NULL, NULL) = 32
[pid 171115] clone3({flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|..., ...}) = 171130
[pid 171130] mq_notify(3, {..., sigev_notify=SIGEV_THREAD, ...}) = 0
...
```

1. Alla prima ``mq_notify`` con ``SIGEV_THREAD`` la libreria apre un socket *netlink* (un canale di comunicazione tra kernel e processo) e crea un thread di servizio che resta bloccato a leggerlo (``recvfrom``). È il secondo thread visto con ``ps -L``, fermo in ``skb_wait_for_more_packets``.
2. Poi chiama la system call ``mq_notify``, passando al posto del segnale il numero del socket. ``strace`` stampa ``SIGILL`` solo perché il socket è il descrittore 4, e 4 è anche il numero del segnale ``SIGILL``.
3. Quando un messaggio arriva nella coda vuota, il kernel scrive 32 byte sul socket. Il thread di servizio si sveglia e crea con ``clone3`` un thread nuovo che esegue ``gestore_notifica``; la ``mq_notify`` successiva è la ri-registrazione fatta dal gestore.

Quindi anche qui c'è un thread fermo ad aspettare: è quello della libreria, uno solo per tutto il processo. Con ``SIGEV_SIGNAL``, o con ``sevp == NULL``, la funzione della libreria chiama invece direttamente la system call.

## Riferimenti

- ``man 3 mq_notify`` (anche online: <https://man7.org/linux/man-pages/man3/mq_notify.3.html>): le regole e un programma di esempio con ``SIGEV_THREAD``.
- ``man 7 mq_overview``: il contenuto dei file in ``/dev/mqueue``.
- ``man sigevent``: la ``struct sigevent``. Il numero di sezione dipende dalla versione delle man page installate: ``man 7 sigevent`` su Ubuntu 22.04, ``man 3type sigevent`` nelle versioni più recenti (ad esempio Ubuntu 24.04).
- ``man 3 mq_close``: la chiusura del descrittore cancella anche la registrazione.
