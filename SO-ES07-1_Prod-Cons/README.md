# SO-ES7-1_Prod-Cons

In questa lezione esercitativa viene affrontato il problema produttore-consumatore in applicazioni multiprocesso. I dati sono memorizzati in shared memory POSIX e la sincronizzazione è realizzata mediante semafori POSIX process-shared.

## Vincoli del problema

Un produttore non deve sovrascrivere una posizione ancora occupata; un consumatore non deve leggere una posizione vuota. Quando esistono più produttori o consumatori, occorre inoltre proteggere gli indici e lo stato della struttura dati condivisa.

Negli esempi vengono utilizzati due tipi di semaforo:

- semafori contatori, per rappresentare il numero di spazi liberi e di messaggi disponibili;
- un semaforo binario, usato come mutex per proteggere le modifiche alla struttura condivisa.

## Sommario degli esempi

- [1_prodcons_singolo_buffer](1_prodcons_singolo_buffer): produttore-consumatore con un singolo buffer;
- [2_prodcons_coda_circolare](2_prodcons_coda_circolare): coda FIFO circolare con più produttori e consumatori;
- [3_prodcons_vettore_stato](3_prodcons_vettore_stato): pool di buffer con stato `LIBERO`/`OCCUPATO`;
- [4_esercizi](4_esercizi): esercizi aggiuntivi da implementare con primitive POSIX.

## Ciclo di vita comune

Il padre crea e dimensiona una shared memory, inizializza al suo interno i semafori con `pshared=1`, genera i processi figli e ne attende la terminazione. Solo dopo l'ultimo `wait` distrugge i semafori, rimuove il mapping e invoca `shm_unlink`.
