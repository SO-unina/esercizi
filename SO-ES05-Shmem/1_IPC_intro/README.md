# Introduzione alle risorse IPC POSIX

L'esempio introduce una risorsa IPC POSIX nominata utilizzando una shared memory. Il programma crea un oggetto, lo dimensiona, lo mappa, scrive una stringa nell'area condivisa e infine rilascia tutte le risorse.

## Obiettivo didattico

Comprendere la differenza tra il nome della risorsa, il descrittore restituito da `shm_open` e l'indirizzo virtuale restituito da `mmap`, osservando il ciclo di vita completo della shared memory POSIX.

## Primitive e strutture utilizzate

- `shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600)`: crea un nuovo oggetto nominato e fallisce con `EEXIST` se il nome è già presente.
- `ftruncate(fd, size)`: porta l'oggetto dalla dimensione iniziale di zero byte alla dimensione richiesta.
- `mmap(..., PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)`: crea un mapping condiviso leggibile e scrivibile.
- `munmap`, `close` e `shm_unlink`: rimuovono rispettivamente il mapping, il descrittore e il nome della risorsa.

## Struttura dei file

- `posix_ipc_intro.c`: contiene il programma completo e gestisce esplicitamente gli errori restituiti dalle primitive POSIX.
- `Makefile`: compila l'eseguibile `posix_ipc_intro`.

## Funzionamento

1. Il processo invoca `shm_open` con `O_CREAT | O_EXCL`, in modo da non riutilizzare accidentalmente una risorsa rimasta da una precedente esecuzione.
2. La dimensione viene impostata a 4096 byte. Senza `ftruncate`, l'oggetto avrebbe dimensione zero e non potrebbe essere usato in modo sicuro.
3. `mmap` restituisce un indirizzo nello spazio virtuale del processo. Con `MAP_SHARED`, le modifiche sarebbero visibili anche ad altri processi che mappassero lo stesso oggetto.
4. Il programma scrive una stringa contenente il proprio PID e la stampa rileggendola dal mapping.
5. Al termine, il mapping viene rimosso, il descrittore viene chiuso e il nome viene eliminato.

## Compilazione

```bash
make
```

Il `Makefile` compila con `gcc` e in fase di collegamento include le librerie richieste dalle primitive POSIX impiegate.

## Esecuzione

```bash
./posix_ipc_intro
```

Un possibile output è:

```text
Messaggio scritto nella shared memory POSIX dal PID 12345
```

## Terminazione e cleanup

Il programma esegue `munmap`, `close` e `shm_unlink`. Se viene interrotto prima di `shm_unlink`, il nome `/so_es05_intro` può rimanere presente. In tal caso una successiva esecuzione con `O_EXCL` segnala che la risorsa esiste già.

## Osservazioni

- La shared memory POSIX non è identificata tramite chiavi numeriche: i processi cooperanti devono conoscere lo stesso nome.
- La modalità `0600` consente lettura e scrittura soltanto al proprietario.
- `shm_unlink` può essere invocata anche subito dopo la creazione se non è necessario che altri eseguibili aprano la risorsa per nome; il mapping già esistente rimane valido.

## Domande di verifica

- Qual è la differenza tra `fd` e il puntatore `area`?
- Perché `ftruncate` deve precedere l'accesso al mapping?
- Cosa accade sostituendo `MAP_SHARED` con `MAP_PRIVATE`?
- Perché l'uso di `O_EXCL` aiuta a rilevare risorse residue?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
