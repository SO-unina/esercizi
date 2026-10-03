# Esercizi sul problema produttore-consumatore

- [**1_prenotazione_teatro**](1_prenotazione_teatro): prenotazione posti in un teatro con vettore di stato (soluzione svolta, shared memory POSIX + semaforo anonimo process-shared).
- [**2_simulazione_accesso_disco**](2_simulazione_accesso_disco): coda circolare di richieste a un disco (soluzione svolta).
- [**2_simulazione_accesso_disco_TODO**](2_simulazione_accesso_disco_TODO): stessa traccia con le chiamate POSIX da completare.

## Tracce aggiuntive

Reimplementare i seguenti esercizi utilizzando shared memory POSIX e semafori POSIX. Per ogni soluzione, collocare nella shared memory sia i dati condivisi sia i semafori anonimi inizializzati con `pshared=1`.

## Esercizio 1 - Due flussi specializzati

Realizzare una coppia di buffer singoli con due processi produttori specializzati e un processo consumatore. Ciascun produttore deposita esclusivamente nel proprio buffer; il consumatore deve prelevare correttamente da entrambi senza leggere un buffer vuoto e senza impedire inutilmente al secondo produttore di avanzare.

Individuare:

- i semafori che rappresentano lo stato libero/occupato di ciascun buffer;
- l'eventuale necessità di un mutex;
- una politica con cui il consumatore sceglie il prossimo buffer.

## Esercizio 2 - Produttori con priorità

Realizzare produttori urgenti e ordinari. Un produttore ordinario può accedere a un buffer libero soltanto quando non vi sono produttori urgenti in attesa. La soluzione deve evitare busy waiting e rendere esplicito il numero di produttori urgenti sospesi.

Verificare se la politica scelta può provocare starvation dei produttori ordinari.

## Esercizio 3 - Prelievo multiplo

Realizzare un pool di buffer nel quale il consumatore possa prelevare fino a `k` elementi con una sola operazione. Specificare se l'operazione deve attendere almeno un elemento oppure esattamente `k` elementi e aggiornare coerentemente i semafori contatori.

## Requisiti POSIX

- I nomi di eventuali risorse nominate devono iniziare con `/`.
- Il creatore deve eseguire `ftruncate` prima di `mmap`.
- I semafori anonimi tra processi devono trovarsi in memoria condivisa.
- Il padre deve attendere tutti i figli prima di `sem_destroy`.
- Ogni esecuzione deve terminare con `munmap`, `close` e `shm_unlink`.

Per ciascun esercizio fornire un `Makefile`, una breve descrizione dell'invariante di sincronizzazione e almeno un controllo automatico sul risultato.
