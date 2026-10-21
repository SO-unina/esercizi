# Cooperazione tra eseguibili distinti: shm nominata e semaforo nominato

Il `master` crea una shared memory nominata per i dati e un semaforo nominato per la cooperazione, poi lancia con `fork` + `exec` due eseguibili: il `lettore`, che si blocca in attesa del dato, e lo `scrittore`, che lo deposita e segnala.

## Obiettivo didattico

Mostrare lo scenario con due risorse nominate: la shm contiene solo i dati, mentre il semaforo vive fuori dalla struct come oggetto nominato a sé. Il semaforo qui non protegge una sezione critica: inizializzato a 0, scandisce un ordine (cooperazione), come il "via" tra scrittore e lettore.

## Struttura dei file

- `condiviso.h`: definisce i due nomi (`SHM_NAME` e `SEM_PRONTO`) e la struct dei dati.
- `master.c`: crea shm e semaforo (valore iniziale 0), lancia prima il lettore e poi lo scrittore, attende e rimuove entrambi i nomi.
- `scrittore.c`: riapre le risorse per nome, deposita il valore nella shm e fa `sem_post`.
- `lettore.c`: riapre le risorse per nome, resta bloccato in `sem_wait` finché il dato non è pronto, poi lo legge.
- `Makefile`: genera `master`, `scrittore` e `lettore`.

## Esecuzione

```bash
make
./master
```

Il lettore parte per primo e stampa "Attendo il dato...": l'attesa rende visibile la cooperazione. Durante l'esecuzione le due risorse sono in `/dev/shm` (il semaforo con il prefisso `sem.`).

## Osservazioni

- Due nomi da concordare invece di uno: rispetto allo scenario con il semaforo anonimo dentro la shm, tenere il semaforo fuori dalla struct ha senso quando il suo ciclo di vita è indipendente dai dati o quando i programmi sono scritti da autori diversi.
- Il valore iniziale 0 realizza la cooperazione; con valore iniziale 1 lo stesso semaforo realizzerebbe la mutua esclusione.

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
