# Esercizio - Coppia di valori condivisa

Due scrittori aggiornano una coppia di interi `(a, b)` mantenendo l'invariante `b == 2 * a`. Quattro lettori controllano ripetutamente che la coppia osservata sia coerente.

## Obiettivo didattico

Mostrare perché un aggiornamento composto da più scritture deve essere osservato atomicamente dai lettori, anche quando ciascun singolo campo è di tipo intero.

## Primitive e strutture utilizzate

- `tornello` impedisce a nuovi lettori di superare uno scrittore in attesa.
- `mutex_lettori` protegge il contatore dei lettori.
- `stanza_vuota` garantisce l'esclusione tra scrittori e gruppo dei lettori.
- La struttura completa è collocata in shared memory POSIX con nome `SHM_NAME`.

## Struttura dei file

- `main.c`: contiene struttura condivisa, primitive di lock per lettori e scrittori, creazione dei processi e controllo finale.
- `Makefile`: genera `main`.

## Funzionamento

1. Il padre inizializza tre semafori process-shared e i valori `a` e `b`.
2. Ciascuno dei due scrittori acquisisce il lock di scrittura e assegna prima `a=k` e poi `b=2*k`.
3. Ciascuno dei quattro lettori acquisisce il lock di lettura e verifica l'invariante cinquanta volte.
4. Più lettori possono controllare contemporaneamente la coppia; nessun lettore può osservare l'aggiornamento intermedio di uno scrittore.
5. Il padre verifica gli status di uscita dei sei figli e stampa l'esito complessivo.

## Compilazione

```bash
make
```

Il `Makefile` compila con `gcc` e in fase di collegamento include le librerie richieste dalle primitive POSIX impiegate.

## Esecuzione

```bash
./main
```

L'esecuzione corretta termina con:

```text
Invariante rispettato: si
```

## Terminazione e cleanup

Il padre distrugge i tre semafori dopo la terminazione di tutti i figli, quindi rimuove mapping, descrittore e nome POSIX.

## Osservazioni

- L'invariante può essere violato senza sincronizzazione anche se lettura e scrittura di un singolo `int` sono atomiche, perché la coppia viene aggiornata con due operazioni distinte.
- Il test è auto-verificante: un lettore termina con errore se osserva uno stato incoerente.

## Domande di verifica

- Quale stato intermedio potrebbe osservare un lettore senza lock?
- È sufficiente usare due mutex distinti, uno per `a` e uno per `b`?
- Come si potrebbe rappresentare la coppia con versioning o seqlock?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
