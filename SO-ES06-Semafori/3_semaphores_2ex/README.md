# Esercizio 2 - Ricerca parallela del minimo

Il programma distribuisce tra dieci processi figli la ricerca del valore minimo in un vettore di 10.000 interi. Il vettore, il minimo globale e il semaforo sono contenuti in una shared memory POSIX.

## Obiettivo didattico

Separare il calcolo locale, che non richiede sincronizzazione, dall'aggiornamento breve di un risultato globale condiviso, che deve essere eseguito in mutua esclusione.

## Primitive e strutture utilizzate

- `shm_open`, `ftruncate` e `mmap` per allocare l'intera struttura condivisa.
- `sem_init(..., 1, 1)`, `sem_wait` e `sem_post` per il mutex process-shared.
- `fork` per generare dieci processi e `wait` per attenderne la terminazione.
- Un calcolo sequenziale nel padre per controllare il risultato ottenuto in parallelo.

## Struttura dei file

- `main.c`: inizializza il vettore, crea i figli, calcola il valore di controllo e verifica il risultato.
- `processi.h`: definisce dimensione del vettore, numero di figli, struttura condivisa e prototipo della funzione di ricerca.
- `processi-mutua-esclusione.c`: calcola il minimo locale della porzione assegnata e aggiorna il minimo globale in sezione critica, chiamando direttamente `sem_wait` e `sem_post`.
- `Makefile`: compila i moduli e genera `main`.

## Funzionamento

1. Il padre inizializza il vettore con valori casuali non negativi e imposta il minimo globale a `INT_MAX`.
2. Ogni figlio riceve implicitamente un indice e analizza una porzione disgiunta del vettore.
3. La scansione della porzione avviene senza mutex, perché nessun altro processo modifica il vettore.
4. Soltanto il confronto e l'eventuale aggiornamento del minimo globale sono protetti dal semaforo.
5. Dopo aver atteso i figli, il padre confronta il risultato con un minimo calcolato sequenzialmente.

## Compilazione

```bash
make
```

Il `Makefile` compila con `gcc` e in fase di collegamento include le librerie richieste dalle primitive POSIX impiegate.

## Esecuzione

```bash
./main
```

L'output riporta il minimo parallelo e il minimo di controllo. I due valori devono coincidere.

## Terminazione e cleanup

Il semaforo viene distrutto dopo la terminazione dei dieci figli. Il padre rimuove mapping, descrittore e nome della shared memory.

## Osservazioni

- Proteggere l'intera scansione con il mutex sarebbe corretto ma eliminerebbe gran parte del parallelismo.
- La riduzione in due fasi, minimo locale seguito da aggiornamento globale, riduce la contesa.
- Il controllo sequenziale rende l'esempio auto-verificante.

## Domande di verifica

- Qual è la sezione critica minima necessaria?
- Come cambierebbe il programma usando un minimo locale per ogni figlio e una riduzione finale nel padre?
- Cosa accade se la dimensione del vettore non è divisibile per il numero dei figli?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
