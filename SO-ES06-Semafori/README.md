# SO-ES6-Semafori

In questa lezione esercitativa vengono affrontati i semafori POSIX per la sincronizzazione tra processi. Un semaforo non trasporta dati: controlla l'ordine di esecuzione e l'accesso a risorse condivise.

## Tipi di semaforo POSIX

Gli esempi mostrano entrambe le forme previste dall'interfaccia POSIX:

- **semaforo nominato**, creato con `sem_open` e identificato da un nome che inizia con `/`; può essere aperto da eseguibili indipendenti e viene rimosso con `sem_unlink`;
- **semaforo anonimo process-shared**, memorizzato in un'area di shared memory e inizializzato con `sem_init(..., 1, valore_iniziale)`. Il valore `1` del parametro `pshared` indica che il semaforo è condiviso tra processi.

Le operazioni principali sono `sem_wait`, che decrementa il contatore o blocca il processo, e `sem_post`, che incrementa il contatore e può risvegliare un processo in attesa.

## Quando serve un nome: i quattro scenari

* **Caso 1: solo fork.** Memoria anonima (`MAP_SHARED | MAP_ANONYMOUS`) e semafori con `sem_init` dentro: nessun nome, i figli ereditano tutto. Esempio: produttori e consumatori generati tutti dallo stesso padre, o figli che aggiornano un contatore in mutua esclusione.
* **Caso 2: eseguibili distinti con dati condivisi.** Shm nominata (`shm_open`), semafori anonimi dentro la struct: chi arriva rifà `shm_open` e `mmap` e la `sem_open` non serve. Esempio: un prod-cons dove produttore e consumatore sono programmi diversi, o un visualizzatore che si aggancia ai dati di un'applicazione già in esecuzione.
* **Caso 3: eseguibili distinti senza dati condivisi.** Il nome sta sul semaforo: `sem_open` in tutti, `sem_unlink` nel responsabile. Esempio: pura mutua esclusione su una risorsa esterna, ad esempio più programmi che scrivono a turno sulla stessa stampante o sullo stesso file, senza alcuna struttura dati in comune.
* **Caso 4: eseguibili distinti, dati condivisi e semafori nominati per cooperazione.** La shm nominata contiene solo i dati, mentre i semafori vivono fuori dalla struct come oggetti nominati a sé: `sem_open` in tutti i processi, `sem_unlink` nel responsabile. Esempio: un prod-cons tra programmi diversi dove il buffer sta nella shm e una coppia di semafori nominati (`vuoto` = DIM, `pieno` = 0) scandisce la cooperazione.

La regola che unifica i quattro casi: serve almeno una risorsa **nominata** per ogni rendezvous tra processi non imparentati, e un solo oggetto nominato può trasportare tutto il necessario. In repo: il caso 1 è [1_semaphores_intro/process_shared_sem.c](1_semaphores_intro/process_shared_sem.c), il caso 2 è [4_shm_nominata_exec](4_shm_nominata_exec) (e ricorre nella prenotazione del teatro di ES07-1), il caso 3 è [5_mutua_esclusione_exec](5_mutua_esclusione_exec), il caso 4 è [6_shm_e_semaforo_nominati](6_shm_e_semaforo_nominati). Negli esercizi delle lezioni successive la shared memory nominata viene usata anche in scenari solo-fork: è una scelta deliberata, per esercitare l'API completa fin dall'inizio.

## Sommario degli esempi

- [1_semaphores_intro](1_semaphores_intro): confronto tra semafori nominati e semafori anonimi process-shared;
- [2_semaphores_1ex](2_semaphores_1ex): mutua esclusione sull'incremento di una variabile condivisa;
- [3_semaphores_2ex](3_semaphores_2ex): ricerca parallela del minimo con aggiornamento protetto del risultato globale;
- [4_shm_nominata_exec](4_shm_nominata_exec): eseguibili distinti che condividono un contatore in una shm nominata, con il semaforo anonimo dentro la struct;
- [5_mutua_esclusione_exec](5_mutua_esclusione_exec): mutua esclusione tra eseguibili distinti lanciati con fork + exec, tramite semaforo nominato;
- [6_shm_e_semaforo_nominati](6_shm_e_semaforo_nominati): cooperazione tra eseguibili distinti con shm nominata per i dati e semaforo nominato per la segnalazione.

## Regole di cleanup

Un semaforo anonimo viene distrutto mediante `sem_destroy` soltanto quando nessun processo lo sta più utilizzando. Un semaforo nominato richiede `sem_close` in ciascun processo e `sem_unlink` da parte del processo responsabile del suo ciclo di vita.

Ogni sottocartella contiene un `Makefile`: eseguire `make` per compilare e `make clean` per rimuovere i prodotti della compilazione.
