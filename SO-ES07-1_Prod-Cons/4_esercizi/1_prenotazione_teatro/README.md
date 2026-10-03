### Esercizio. Produttore/consumatore con vettore di stato e semafori


Si realizzi in linguaggio C/C++ un'applicazione multiprocesso per la simulazione di un servizio di prenotazione posti in un teatro. L'applicazione è costituita da 51 processi, 50 clienti e 1 visualizzatore. I posti nel teatro sono rappresentati da un vettore di 80 elementi, ognuno contenente la seguente struttura posto:

```c
typedef struct {
    unsigned int id_cliente;
    unsigned int stato; 
} posto;
```

dove ``id_cliente`` è il PID del cliente che ha prenotato il posto (inizialmente 0), e stato indica se il posto è libero (0), occupato (1), o in_aggiornamento (2) da parte di un altro cliente. Una variabile _disponibilità_ indica il numero di posti ancora presenti in teatro (inizialmente 80).

Ognuno dei clienti dapprima attende un tempo casuale da 0 a 5 secondi (con una sleep), poi effettua un tentativo di prenotazione da 1 a 4 posti (il numero di posti e i secondi di attesa possono essere generati in maniera casuale con con la funzione ``rand()``[1]). Ad ogni tentativo di prenotazione, il client stabilisce se c'è spazio, in base alla variabile disponibilità, altrimenti termina subito con un messaggio di "disponibilità esaurita". Se c'è disponiblità, pone in aggiornamento i primi posti liberi che trova, li occupa, assegnando ai posti il proprio PID in id_cliente (si simuli con una sleep un tempo di 1 secondo per occupare i posti), ed aggiorna la disponibilità.

Il processo visualizzatore stampa a video, ogni secondo, la situazione dei posti nel teatro, scrivendo su ogni riga il numero di posto, lo stato, ed eventualmente il cliente che ha occupato il posto.

Il vettore dei posti e la variabile disponibilità sono **variabili condivise**, e l'accesso a tali variabili da parte dei processi deve essere disciplinato attraverso l'uso di semafori POSIX (semaforo anonimo process-shared collocato nella shared memory POSIX).

I processi clienti sono generati da un programma principale attraverso la primitiva ``fork()``. Una volta generati i processi, il programma principale ne attende la terminazione e termina a sua volta. Il processo visualizzatore venga invece implementato in un programma principale a parte.

[1]: La generazione casuale può essere implementata con la funzione ``rand()`` di ``stdlib.h``; ad esempio: ``int num = rand() % 4 +1``. ``rand()`` richiede che venga generato un seme dei numeri casuali attraverso la funzione ``srand(time(NULL))``.

**Nota POSIX:** la memoria condivisa è creata dal programma `clienti` con `shm_open` + `ftruncate` + `mmap` (nome `/so_es07_teatro`, visibile in `/dev/shm`); il `visualizzatore` apre lo stesso nome senza `O_CREAT`. Il mutex è un semaforo anonimo inizializzato con `sem_init(..., pshared=1, ...)` all'interno della shared memory. Al termine, il creatore effettua `sem_destroy` e `shm_unlink`.
