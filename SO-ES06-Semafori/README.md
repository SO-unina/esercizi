# SO-ES6-Semafori

In questa lezione esercitativa vengono affrontati i semafori POSIX per la sincronizzazione tra processi. Un semaforo non trasporta dati: controlla l'ordine di esecuzione e l'accesso a risorse condivise.

## Tipi di semaforo POSIX

Gli esempi mostrano entrambe le forme previste dall'interfaccia POSIX:

- **semaforo nominato**, creato con `sem_open` e identificato da un nome che inizia con `/`; può essere aperto da eseguibili indipendenti e viene rimosso con `sem_unlink`;
- **semaforo anonimo process-shared**, memorizzato in un'area di shared memory e inizializzato con `sem_init(..., 1, valore_iniziale)`. Il valore `1` del parametro `pshared` indica che il semaforo è condiviso tra processi.

Le operazioni principali sono `sem_wait`, che decrementa il contatore o blocca il processo, e `sem_post`, che incrementa il contatore e può risvegliare un processo in attesa.

## Sommario degli esempi

- [1_semaphores_intro](1_semaphores_intro): confronto tra semafori nominati e semafori anonimi process-shared;
- [2_semaphores_1ex](2_semaphores_1ex): mutua esclusione sull'incremento di una variabile condivisa;
- [3_semaphores_2ex](3_semaphores_2ex): ricerca parallela del minimo con aggiornamento protetto del risultato globale;
- [4_mutua_esclusione_exec](4_mutua_esclusione_exec): mutua esclusione tra eseguibili distinti lanciati con fork + exec, tramite semaforo nominato.

## Regole di cleanup

Un semaforo anonimo viene distrutto mediante `sem_destroy` soltanto quando nessun processo lo sta più utilizzando. Un semaforo nominato richiede `sem_close` in ciascun processo e `sem_unlink` da parte del processo responsabile del suo ciclo di vita.

Ogni sottocartella contiene un `Makefile`: eseguire `make` per compilare e `make clean` per rimuovere i prodotti della compilazione.
