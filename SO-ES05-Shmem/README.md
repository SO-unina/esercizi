# SO-ES5-Shmem

In questa lezione esercitativa vengono affrontate le risorse per la comunicazione inter-processo (IPC). In particolare, si studia la shared memory POSIX, che permette a processi distinti di mappare nel proprio spazio di indirizzamento lo stesso oggetto di memoria.

## Obiettivi

Gli esempi mostrano il ciclo di vita completo di una risorsa di shared memory POSIX:

1. creazione o apertura dell'oggetto mediante `shm_open`;
2. definizione della dimensione mediante `ftruncate`;
3. associazione dell'oggetto allo spazio di indirizzamento mediante `mmap` con `MAP_SHARED`;
4. lettura e scrittura dei dati tramite normali accessi in memoria;
5. rimozione del mapping mediante `munmap`;
6. chiusura del descrittore mediante `close`;
7. rimozione del nome mediante `shm_unlink`.

La shared memory realizza la comunicazione, ma non fornisce automaticamente la sincronizzazione. Quando più processi possono accedere contemporaneamente agli stessi dati, occorre utilizzare semafori o altri meccanismi di sincronizzazione.

## Sommario degli esempi

- [1_IPC_intro](1_IPC_intro): introduzione al ciclo di vita di un oggetto IPC POSIX nominato;
- [2_shmem](2_shmem): uso della shared memory tra processi creati con `fork` e tra eseguibili distinti;
- [3_shmem_err_examples](3_shmem_err_examples): errori frequenti relativi a dimensionamento, persistenza del nome e dimensione del mapping.

## Nomi e persistenza

Un nome POSIX deve iniziare con `/`, ad esempio `/so_es05_intro`. `shm_unlink` rimuove il nome, impedendo nuove aperture, ma l'oggetto viene eliminato effettivamente solo quando non esistono più descrittori aperti e mapping attivi.

Ogni sottocartella contiene un `Makefile`. Entrare nella cartella desiderata ed eseguire `make`; utilizzare `make clean` al termine.
