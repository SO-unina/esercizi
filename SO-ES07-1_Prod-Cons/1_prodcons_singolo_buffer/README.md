## Produttore-Consumatore singolo buffer

Nel problema produttore-consumatore, abbiamo due categorie di processi:

- **Produttori**, che scrivono un messaggio su di una risorsa condivisa
- **Consumatori**, che prelevano il messaggio dalla risorsa condivisa

Pur esistendo un problema (potenziale) di mutua esclusione nell'utilizzo del buffer comune, la soluzione impone un ordinamento nelle operazioni dei due processi. E' necessario che produttori e consumatori si scambino segnali per indicare rispettivamente l'avvenuto deposito e prelievo.
Ciascuno dei due processi deve attendere, per completare la sua azione, l'arrivo del segnale dell'altro processo.

### Esercizio

*Scrivere un'applicazione concorrente che implementi il problema dei Produttori/Consumatori.
Il programma crei dei processi che agiscano da produttore e consumatore utilizzando un unico buffer di memoria in cui sono memorizzati valori di tipo intero. Il buffer di memoria è creato attraverso una shared memory POSIX (`shm_open`, `ftruncate`, `mmap`) e la sincronizzazione tra produttori e consumatori deve avvenire tramite l'utilizzo di semafori POSIX anonimi (`sem_init`, `sem_wait`, `sem_post`).*

I vincoli che caratterizzano il problema produttore-consumatore a singolo buffer sono i seguenti:

- Il produttore non può produrre un messaggio prima che qualche consumatore abbia letto il messaggio precedente.
- Il consumatore non può prelevare alcun messaggio fino a che un produttore non l'abbia depositato.

<p align="center">
<img src="../images/prod_cons_single_buffer.png" width="400">
</p>

Per la sincronizzazione dei processi produttore e consumatore si utilizzano due semafori, campi `sem_t` della struttura condivisa `struct prodcons`:

- ``spazio_disponibile``: semaforo bloccato da un produttore prima di una produzione, e sbloccato da un consumatore in seguito ad un consumo. Il valore iniziale del semaforo deve essere pari ad ``1``;

- ``messaggio_disponibile``: semaforo sbloccato da un produttore in seguito ad una produzione, e bloccato da un consumatore prima del consumo. Il valore iniziale del semaforo deve essere pari ad ``0``.

Non serve alcun mutex: con un solo produttore e un solo consumatore i due semafori di cooperazione impongono già l'alternanza.

La struttura condivisa, che contiene sia il buffer (un solo `int`) sia i semafori, è:

```c
struct prodcons {
    sem_t spazio_disponibile;
    sem_t messaggio_disponibile;
    int valore;
};
```

La produzione ed il consumo avvengono rispettivamente all'interno delle procedure:

```c
void produttore(struct prodcons *);
void consumatore(struct prodcons *);
```

dove l'argomento è un puntatore alla struttura condivisa mappata con `mmap`: tramite esso le procedure accedono sia al buffer sia ai semafori, sui quali effettuano le operazioni di wait (i.e., ``sem_wait``) e signal (i.e., ``sem_post``) necessarie per la cooperazione tra produttore e consumatore.
Il valore prodotto è un intero generato tramite la funzione ``rand()``.

Analizzare il file [procedure.c](procedure.c) in cui vengono implementate le funzioni di produzione e consumazione e il file [prodcons_singolo_buffer.c](prodcons_singolo_buffer.c) dove vengono creati la shared memory e i semafori necessari (e, al termine, distrutti con `sem_destroy`, `munmap` e `shm_unlink`).
Compilare ed eseguire il codice:

```console
$ make
$ ./prodcons_singolo_buffer
Inizio figlio consumatore
consumatore è fermo prima di wait
Inizio figlio produttore
produttore è fermo prima di wait
produttore si sblocca dopo la wait
Il valore prodotto = 8
consumatore si sblocca dopo la wait
primo figlio terminato
Il valore consumato = 8
secondo figlio terminato
```
