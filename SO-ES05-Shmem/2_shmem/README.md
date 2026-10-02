# Utilizzo della shared memory POSIX

Gli esempi mostrano due modalità di condivisione della stessa area di memoria: un mapping creato prima di `fork` e quindi ereditato dal processo figlio, oppure un oggetto nominato aperto indipendentemente da due eseguibili distinti.

## Obiettivo didattico

Confrontare l'ereditarietà dei mapping dopo `fork` con l'apertura esplicita di una risorsa nominata mediante `shm_open`, distinguendo anche i permessi di mapping in sola lettura e in lettura/scrittura.

## Primitive e strutture utilizzate

- `shm_open`, `ftruncate` e `mmap` per creare e mappare l'oggetto condiviso.
- `fork` e `wait` nell'esempio a singolo eseguibile.
- `PROT_READ | PROT_WRITE` per chi modifica i dati e `PROT_READ` per il lettore.
- `munmap`, `close` e `shm_unlink` per la terminazione.

## Struttura dei file

- `shared_data.h`: definisce il nome condiviso e la struttura `shared_data_t`, composta da un intero e una stringa.
- `single_executable.c`: crea un mapping, esegue `fork` e mostra che le modifiche del figlio sono visibili al padre.
- `writer.c`: crea l'oggetto nominato, pubblica i dati e attende che venga avviato il lettore.
- `reader.c`: apre lo stesso oggetto in sola lettura e stampa i dati pubblicati.
- `Makefile`: genera `single_executable`, `writer` e `reader`.

## Funzionamento

1. `single_executable` genera un nome univoco basato sul PID, crea e dimensiona la shared memory e inizializza la struttura.
2. Dopo `fork`, padre e figlio possiedono mapping distinti nei rispettivi spazi virtuali, ma entrambi fanno riferimento allo stesso oggetto sottostante. Il figlio modifica i dati e il padre li legge dopo `wait`.
3. Nella variante con eseguibili distinti, `writer` crea `/so_es05_shared_data` e rimane attivo in attesa di Invio.
4. `reader` apre lo stesso nome con `O_RDONLY`, crea un mapping `PROT_READ` e stampa i campi della struttura.
5. Il processo `writer`, responsabile della creazione, elimina il nome al termine.

## Compilazione

```bash
make
```

Il `Makefile` compila con `gcc` e in fase di collegamento include le librerie richieste dalle primitive POSIX impiegate.

## Esecuzione

Esempio con `fork`:

```bash
./single_executable
```

Esempio con due eseguibili, usando due terminali:

```bash
# Terminale 1
./writer

# Terminale 2, mentre writer è ancora in attesa
./reader
```

Dopo l'esecuzione di `reader`, premere Invio nel terminale di `writer`.

## Terminazione e cleanup

`single_executable` rimuove il proprio nome dopo la terminazione del figlio. Nella seconda variante, `writer` esegue preventivamente `shm_unlink` per eliminare eventuali residui, quindi rimuove nuovamente il nome al termine. `reader` non esegue `shm_unlink` perché non è il proprietario del ciclo di vita della risorsa.

## Osservazioni

- L'ordine di avvio è significativo: `reader` fallisce se `writer` non ha ancora creato la risorsa.
- Il mapping ereditato dopo `fork` non richiede una nuova `shm_open` nel figlio.
- La sincronizzazione è ottenuta qui con `wait` o con l'interazione manuale; la shared memory da sola non impedisce accessi concorrenti.

## Domande di verifica

- Perché il lettore può utilizzare `O_RDONLY` e `PROT_READ`?
- Cosa accadrebbe se `writer` eseguisse `shm_unlink` subito dopo `mmap`?
- Quale sincronizzazione sarebbe necessaria se `writer` aggiornasse continuamente la struttura?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
