# Shared memory nominata con semaforo anonimo, tra eseguibili distinti

Il `master` crea una shared memory nominata che contiene un contatore e un semaforo anonimo, poi lancia con `fork` + `exec` due istanze dell'eseguibile `lavoratore`, che incrementano il contatore in mutua esclusione.

## Obiettivo didattico

Mostrare lo scenario "eseguibili distinti con dati condivisi": basta un solo nome, quello della shm. Il semaforo è anonimo dentro la struct e chi arriva dopo la `exec` lo ritrova con `shm_open` + `mmap`, senza alcuna `sem_open`.

## Struttura dei file

- `condiviso.h`: definisce `SHM_NAME` e la struct con contatore e `sem_t`.
- `master.c`: crea e inizializza la risorsa, lancia due lavoratori, attende, stampa il valore finale e rimuove il nome.
- `lavoratore.c`: riapre la shm per nome (senza `O_CREAT`) ed esegue gli incrementi sotto il mutex.
- `Makefile`: genera `master` e `lavoratore`.

## Esecuzione

```bash
make
./master
```

Valore finale atteso: 10 (2 lavoratori per 5 incrementi). Lanciare `./lavoratore` da solo produce l'errore di `shm_open`: la risorsa la crea il master.

## Osservazioni

- La `exec` distrugge i mapping ereditati, non gli oggetti: per questo il lavoratore deve rifare `shm_open` e `mmap`, e per questo la shm deve essere nominata.
- Il semaforo viaggia "gratis" dentro la struct: un `sem_t` in memoria condivisa funziona per chiunque mappi l'oggetto, non solo per chi ha eseguito la `sem_init`.

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
