# Lettori-scrittori con possibile starvation degli scrittori

La soluzione assegna priorità ai lettori. Più lettori possono accedere contemporaneamente al valore condiviso; gli scrittori accedono uno alla volta e soltanto quando non vi sono lettori attivi.

## Obiettivo didattico

Implementare il classico schema first-reader/last-reader e riconoscere la condizione che può provocare starvation degli scrittori.

## Primitive e strutture utilizzate

- `mutex_numlettori` (init 1) protegge il contatore `numlettori`.
- `mutex_lettori_scrittori` (init 1) garantisce l'accesso esclusivo tra il gruppo dei lettori e ciascuno scrittore.
- `shm_open`, `ftruncate` e `mmap` collocano dati e semafori nella struttura condivisa `Buffer`.
- `sem_wait` e `sem_post` sono chiamate direttamente nelle procedure di inizio e fine lettura/scrittura.

## Struttura dei file

- `header.h`: definisce `Buffer`, il nome `SHM_NAME` e l'interfaccia delle procedure.
- `procedure.c`: implementa `InizioLettura`, `FineLettura`, `InizioScrittura`, `FineScrittura`, `Lettore` e `Scrittore`.
- `lett_scrit_starv_scrittori.c`: crea le risorse, genera sei scrittori e sei lettori e svolge il cleanup.
- `Makefile`: genera `lettore_scrittore_exe`.

## Funzionamento

1. Il primo lettore che porta il contatore da 0 a 1 acquisisce `mutex_lettori_scrittori`.
2. I lettori successivi incrementano il contatore e possono accedere senza attendere.
3. L'ultimo lettore, portando il contatore a zero, rilascia `mutex_lettori_scrittori`.
4. Uno scrittore acquisisce direttamente `mutex_lettori_scrittori`, scrive il valore e lo rilascia.
5. Poiché un nuovo lettore può entrare mentre uno scrittore è in attesa, una sequenza continua di lettori può rinviare indefinitamente lo scrittore.

## Compilazione

```bash
make
```

Il `Makefile` compila con `gcc` e in fase di collegamento include le librerie richieste dalle primitive POSIX impiegate.

## Esecuzione

```bash
./lettore_scrittore_exe
```

Il programma crea sei scrittori e sei lettori; ogni scrittore scrive un valore ricavato da `gettimeofday`, ogni lettore stampa il valore corrente e il numero di lettori attivi.

## Terminazione e cleanup

Dopo dodici `wait`, il padre distrugge i due semafori, esegue `munmap` e rimuove il nome con `shm_unlink`.

## Osservazioni

- La mutua esclusione tra lettori non riguarda la lettura della risorsa, ma soltanto l'aggiornamento del loro contatore.
- Il valore condiviso può essere letto contemporaneamente da più processi finché nessuno scrittore possiede `mutex_lettori_scrittori`.

## Domande di verifica

- In quale punto si manifesta la priorità ai lettori?
- Perché solo il primo lettore acquisisce `mutex_lettori_scrittori`?
- Come si può impedire a nuovi lettori di superare uno scrittore già in attesa?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
