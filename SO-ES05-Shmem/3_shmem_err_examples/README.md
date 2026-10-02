# Errori frequenti con la shared memory POSIX

Questa cartella raccoglie tre esempi che mostrano errori ricorrenti nell'uso della shared memory POSIX. I programmi rilevano le condizioni pericolose e le descrivono senza effettuare intenzionalmente accessi non validi.

## Obiettivo didattico

Imparare a verificare dimensione e stato di una risorsa prima del mapping e a gestire correttamente la persistenza dei nomi POSIX.

## Primitive e strutture utilizzate

- `fstat` per interrogare la dimensione corrente dell'oggetto.
- `O_CREAT | O_EXCL` ed `errno == EEXIST` per rilevare una risorsa già esistente.
- `ftruncate` per impostare una dimensione coerente con il mapping richiesto.
- `shm_unlink` per rimuovere risorse residue.

## Struttura dei file

- `no_ftruncate.c`: mostra che un oggetto appena creato ha dimensione zero e non deve essere usato prima di `ftruncate`.
- `stale_object.c`: mostra come `O_EXCL` consenta di rilevare un nome già presente.
- `wrong_mapping_size.c`: confronta la dimensione reale dell'oggetto con la dimensione di mapping richiesta.
- `Makefile`: genera i tre eseguibili omonimi.

## Funzionamento

1. `no_ftruncate` crea l'oggetto, legge `st_size` tramite `fstat` e mostra che la dimensione iniziale è zero.
2. `stale_object` apre una risorsa senza `O_EXCL` e tenta una seconda creazione esclusiva dello stesso nome, verificando l'errore `EEXIST`.
3. `wrong_mapping_size` dimensiona l'oggetto a 128 byte, confronta tale valore con una richiesta di 4096 byte e rifiuta il mapping.
4. Tutti gli esempi chiudono il descrittore e rimuovono il nome prima di terminare.

## Compilazione

```bash
make
```

Il `Makefile` compila con `gcc` e in fase di collegamento include le librerie richieste dalle primitive POSIX impiegate.

## Esecuzione

```bash
./no_ftruncate
./stale_object
./wrong_mapping_size
```

## Terminazione e cleanup

Gli esempi invocano `shm_unlink` per non lasciare oggetti nominati. Se un programma reale termina in modo anomalo, è utile prevedere una procedura di cleanup o usare nomi univoci per esecuzione.

## Osservazioni

- L'accesso a pagine del mapping oltre la dimensione effettiva dell'oggetto può causare `SIGBUS`.
- `mmap` può riuscire anche quando la dimensione richiesta non è coerente con l'oggetto; il problema può emergere soltanto al primo accesso.
- Aprire una risorsa preesistente senza verificarne proprietario, dimensione e protocollo può far utilizzare dati incompatibili.

## Domande di verifica

- Perché un oggetto creato da `shm_open` ha inizialmente dimensione zero?
- Quale differenza c'è tra `O_CREAT` e `O_CREAT | O_EXCL`?
- Perché è utile controllare `st_size` con `fstat` prima del mapping?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
