# Messaggi con priorità

L'esempio invia sei messaggi sulla stessa coda, assegnando priorità 1 ai messaggi ordinari e priorità 5 ai messaggi urgenti. Il destinatario stampa priorità, sequenza e testo.

## Obiettivo didattico

Osservare la politica di selezione delle code POSIX: viene consegnato prima il messaggio con priorità numerica maggiore; a parità di priorità viene preservato l'ordine FIFO.

## Primitive e strutture utilizzate

- Il quarto argomento di `mq_send` assegna una priorità di tipo `unsigned int`.
- Il quarto argomento di `mq_receive` restituisce la priorità del messaggio estratto.
- `mq_attr` configura massimo dieci messaggi e una dimensione pari a `sizeof(message_t)`.
- `mq_open`, `mq_close` e `mq_unlink` gestiscono il ciclo di vita della coda.

## Struttura dei file

- `messages.h`: definisce il nome della coda e la struttura con numero di sequenza e testo.
- `sender.c`: invia sei messaggi con sequenza di priorità `{1, 1, 5, 1, 5, 1}`.
- `receiver.c`: crea la coda, riceve sei messaggi e stampa la priorità restituita.
- `Makefile`: genera `sender` e `receiver`.

## Funzionamento

1. Il destinatario crea la coda e si blocca sulla prima `mq_receive`.
2. Il mittente accoda tutti i messaggi, associando a ciascuno una priorità.
3. Se più messaggi sono presenti contemporaneamente, quelli con priorità 5 vengono estratti prima di quelli con priorità 1.
4. I messaggi della stessa classe mantengono l'ordine relativo di invio.
5. Dopo sei ricezioni, il destinatario chiude e rimuove la coda.

## Compilazione

```bash
make
```

Il `Makefile` compila con `gcc` e in fase di collegamento include le librerie richieste dalle primitive POSIX impiegate.

## Esecuzione

Usare due terminali:

```bash
# Terminale 1
./receiver

# Terminale 2
./sender
```

## Terminazione e cleanup

Il destinatario elimina un eventuale nome residuo prima della creazione e invoca `mq_unlink` al termine. Il mittente chiude soltanto il proprio descrittore.

## Osservazioni

- La priorità POSIX è associata al singolo messaggio, non a una coda separata.
- L'ordine osservato può dipendere dal momento in cui destinatario e mittente vengono schedulati: un messaggio ordinario può essere ricevuto prima che un messaggio urgente sia stato effettivamente inviato.
- Per osservare chiaramente il riordinamento, è utile che più messaggi risultino accodati prima delle ricezioni.

## Domande di verifica

- Le priorità POSIX rappresentano direttamente un tipo di messaggio come in System V?
- Quale ordine viene usato a parità di priorità?
- Come si potrebbe evitare starvation dei messaggi a bassa priorità?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
