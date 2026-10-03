# Esercizio 1 - Incremento in mutua esclusione

Il programma crea due processi figli che incrementano cento volte ciascuno una variabile intera collocata in shared memory POSIX. Un semaforo POSIX process-shared protegge la sequenza di lettura, incremento e scrittura.

## Obiettivo didattico

Osservare una race condition su un aggiornamento non atomico e utilizzare un semaforo binario come mutex tra processi.

## Primitive e strutture utilizzate

- `shm_open`, `ftruncate` e `mmap` per la struttura condivisa.
- `sem_init(&s->mutex, 1, 1)` per inizializzare un semaforo process-shared con valore uno.
- `sem_wait` e `sem_post`, chiamate direttamente per acquisire e rilasciare il mutex.
- `fork` e `wait` per creare e attendere i due processi figli.

## Struttura dei file

- `main.c`: crea la risorsa, genera i figli, verifica il risultato finale e svolge il cleanup; sincronizza i figli chiamando direttamente `sem_wait` e `sem_post`.
- `Makefile`: compila il modulo e genera `main`.

## Funzionamento

1. Il padre crea una struttura condivisa contenente il semaforo e la variabile `valore`, inizialmente zero.
2. Ciascun figlio ripete dieci volte: attesa sul mutex, copia del valore in una variabile locale, incremento, scrittura del risultato e rilascio del mutex.
3. La pausa casuale (`sleep` di 0 o 1 secondi) nel mezzo della sezione critica rende più facile osservare la race condition se si rimuove la sincronizzazione.
4. Il padre attende entrambi i figli e verifica che il risultato sia 20.
5. Solo dopo la terminazione dei figli vengono distrutti il semaforo e la shared memory.

## Compilazione

```bash
make
```

Il `Makefile` compila con `gcc` e in fase di collegamento include le librerie richieste dalle primitive POSIX impiegate.

## Esecuzione

```bash
./main
```

Il programma termina con successo quando stampa un valore finale pari a `20`.

## Terminazione e cleanup

Il padre invoca `sem_destroy` soltanto dopo aver atteso entrambi i figli, quindi esegue `munmap`, `close` e `shm_unlink`.

## Osservazioni

- `valore++` non è un'operazione indivisibile: comprende lettura, modifica e scrittura.
- Il semaforo è memorizzato nella stessa struttura condivisa dei dati protetti.
- Il nome della shared memory contiene il PID del padre per ridurre le collisioni tra esecuzioni contemporanee.

## Domande di verifica

- Cosa succede eliminando `sem_wait`, `sem_post` o entrambe?
- Cosa indica il parametro `pshared` di `sem_init`?
- Perché un semaforo locale nello stack del padre non sarebbe sufficiente?
- Come cambierebbe l'esempio usando eseguibili distinti e un semaforo nominato?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
