# Attesa e terminazione: ``wait()``, ``exit()``, zombie e orfani

In questa cartella ci sono tre piccoli programmi che mostrano che cosa succede quando un processo figlio termina:

| Programma | Cosa mostra |
|---|---|
| [``stato.c``](stato.c) | come il padre legge con ``wait()`` lo stato di terminazione del figlio: codice di ``exit()`` o segnale |
| [``zombie.c``](zombie.c) | un figlio terminato che il padre non ha ancora "raccolto" con ``wait()`` |
| [``orfano.c``](orfano.c) | un figlio che sopravvive al padre e viene adottato da un altro processo |

La teoria su ``wait()`` ed ``exit()`` è nel README di [``2_fork``](../2_fork#attesa-e-terminazione-di-un-processo).

## Esempi da provare

### 1. Compilare

```console
$ make
gcc -Wall    stato.c   -o stato
gcc -Wall    zombie.c   -o zombie
gcc -Wall    orfano.c   -o orfano
```

Il [``Makefile``](Makefile) non contiene regole per i tre programmi: ``make`` usa la sua regola implicita, che produce l'eseguibile ``X`` a partire da ``X.c`` con le variabili ``CC`` e ``CFLAGS``.

### 2. Lo stato di terminazione

Con un argomento, il figlio termina con ``exit(argomento)``. Il padre stampa lo stato restituito da ``wait()`` in esadecimale e poi lo interpreta con le macro.

```console
$ ./stato 42
figlio 216182: termino con exit(42)
padre: wait ha restituito 216182, stato = 0x2a00
padre: il figlio ha chiamato exit, WEXITSTATUS = 42
```

Lo stato è ``0x2a00``: il byte meno significativo vale ``00`` (il figlio ha terminato volontariamente, con ``exit()``) e quello più significativo vale ``0x2a``, cioè 42, il valore passato a ``exit()``. È lo schema della slide "Wait e Exit". ``WEXITSTATUS`` estrae proprio quel byte.

> **_Domanda:_** che cosa stamperà ``./stato 300``?

```console
$ ./stato 300
figlio 216184: termino con exit(300)
padre: wait ha restituito 216184, stato = 0x2c00
padre: il figlio ha chiamato exit, WEXITSTATUS = 44
```

Del valore passato a ``exit()`` arriva al padre solo il byte meno significativo: 300 = 256 + 44. Per questo lo stato di uscita va da 0 a 255 (e per convenzione 0 significa "tutto bene").

### 3. Un figlio ucciso da un segnale

Senza argomenti, il figlio si blocca con ``pause()`` e aspetta un segnale. Lanciamo il programma in background, con ``&``, per poter usare lo stesso terminale:

```console
$ ./stato &
[1] 216185
figlio 216187: aspetto un segnale...
$ ps -o pid,ppid,stat,wchan:20,comm
    PID    PPID STAT WCHAN                COMMAND
 216164  216163 Ss   bash
 216185  216164 S    do_wait              stato
 216187  216185 S    do_sys_pause         stato
 216206  216164 R+   -                    ps
```

Padre e figlio sono entrambi bloccati (stato ``S``): il padre dentro la ``wait()`` (``do_wait``), il figlio dentro la ``pause()``.

Ora mandiamo al figlio il segnale di terminazione (usate il PID che ha stampato il vostro figlio):

```console
$ kill 216187
padre: wait ha restituito 216187, stato = 0x000f
padre: il figlio è stato ucciso da un segnale, WTERMSIG = 15
[1]+  Done                    ./stato
```

Questa volta il byte più significativo è 0 e quello meno significativo vale 15, il numero del segnale ``SIGTERM`` inviato da ``kill``. ``WIFEXITED`` è falsa, ``WIFSIGNALED`` è vera e ``WTERMSIG`` restituisce il numero del segnale. Provate anche con ``kill -9`` (``SIGKILL``).

### 4. Uno zombie

In [``zombie.c``](zombie.c) il figlio termina subito, mentre il padre dorme 30 secondi prima di chiamare ``wait()``.

```console
$ ./zombie &
[1] 216209
padre 216209: dormo 30 secondi senza chiamare wait
figlio 216211: termino subito
$ ps -o pid,ppid,stat,cmd
    PID    PPID STAT CMD
 216164  216163 Ss   bash
 216209  216164 S    ./zombie
 216211  216209 Z    [zombie] <defunct>
 216215  216164 R+   ps -o pid,ppid,stat,cmd
```

Il figlio è nello stato ``Z`` (``<defunct>``): ha già terminato, ma il kernel ne conserva il descrittore con lo stato di terminazione, in attesa che il padre lo raccolga.

> **_Domanda:_** si può eliminare uno zombie con ``kill -9``?

```console
$ kill -9 216211
$ ps -o pid,ppid,stat,cmd
    PID    PPID STAT CMD
 216164  216163 Ss   bash
 216209  216164 S    ./zombie
 216211  216209 Z    [zombie] <defunct>
 216216  216164 R+   ps -o pid,ppid,stat,cmd
```

No: il processo è già morto, e un segnale non può fargli niente. Lo zombie sparisce solo quando il padre chiama ``wait()``: allo scadere dei 30 secondi comparirà

```console
padre: ho chiamato wait, lo zombie 216211 non c'è più
[1]+  Done                    ./zombie
```

> **_N.B.:_** uno zombie non occupa memoria, ma occupa un PID. Un server che crea figli e non chiama mai ``wait()`` accumula zombie, e prima o poi esaurisce i PID disponibili.

### 5. Un orfano

In [``orfano.c``](orfano.c) il padre termina dopo un secondo, senza aspettare il figlio; il figlio stampa il PID del padre prima e dopo.

```console
$ ./orfano
figlio 216466: mio padre è 216465
padre 216465: termino senza aspettare il figlio
$ figlio 216466: ora mio padre è 1
```

Il prompt ricompare appena termina il padre, perché la shell aspettava solo lui; la riga del figlio arriva dopo (premete Invio per riavere il prompt). Il figlio orfano è stato adottato da ``init``, PID 1, che chiamerà ``wait()`` per lui quando terminerà: per questo gli orfani non restano zombie.

> **_N.B.:_** il nuovo padre dipende dal sistema. In Linux un processo può chiedere al kernel di adottare, al posto di ``init``, gli orfani dei propri discendenti: si chiama *subreaper*. Il caso più comune è ``systemd --user``: una seconda istanza di ``systemd``, avviata al login per ogni utente, che gestisce i servizi della sua sessione. È un processo normale, con un PID qualsiasi (non è il PID 1). Con GNOME il terminale è un suo discendente, quindi l'orfano viene adottato da lui; con altri ambienti grafici, da una console testuale o via ``ssh``, viene adottato dal PID 1. Per scoprire chi è il nuovo padre, usate il PID stampato dal figlio:
>
> ```console
> $ ps -o pid,args -p 1845
>     PID COMMAND
>    1845 /usr/lib/systemd/systemd --user
> ```
