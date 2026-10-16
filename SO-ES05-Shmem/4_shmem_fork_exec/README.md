# Shared memory tra eseguibili distinti con master, fork + exec

Il programma `master` crea l'oggetto di shared memory e lancia con `fork` + `exec` due eseguibili distinti: `writer`, che scrive i dati, e `reader`, che li legge. Al termine il master rimuove la risorsa.

## Obiettivo didattico

Separare i due ruoli che un processo può avere rispetto a una risorsa nominata:

- il **creatore** (il master) è responsabile del ciclo di vita: `shm_open` con `O_CREAT | O_EXCL`, `ftruncate` per la dimensione e, alla fine, `shm_unlink`;
- gli **utilizzatori** (writer e reader) riaprono la risorsa per nome con `shm_open` **senza** `O_CREAT`, la usano e rilasciano soltanto il proprio mapping con `munmap`: non toccano il nome.

L'esempio mostra inoltre perché con `exec` il rendezvous deve avvenire per nome: la `exec` sostituisce lo spazio di indirizzamento del figlio e distrugge ogni mapping ereditato dalla `fork`, quindi writer e reader non possono ricevere la memoria condivisa "in eredità" dal master.

## Struttura dei file

- `shared_data.h`: definisce il nome `SHM_NAME` e la struttura `shared_data_t`.
- `master.c`: crea e dimensiona la risorsa, lancia writer e poi reader, attende e rimuove il nome.
- `writer.c`: apre per nome in lettura/scrittura, scrive valore e testo.
- `reader.c`: apre per nome in sola lettura (`O_RDONLY` + `PROT_READ`), stampa i dati.
- `Makefile`: genera `master`, `writer` e `reader`.

## Funzionamento

1. Il master elimina un eventuale oggetto residuo (`shm_unlink` preventivo) e crea la risorsa con `O_CREAT | O_EXCL`.
2. Dopo la `ftruncate`, il master chiude subito il descrittore: non accede ai dati, e l'oggetto continua ad esistere perché il suo nome è registrato nel sistema.
3. Il primo figlio esegue `execl("./writer", ...)`: il writer riapre la risorsa per nome e scrive.
4. Il master attende il writer, poi lancia il reader, che riapre per nome e legge. L'ordine scrittore→lettore è garantito dalle `wait`: in questo esempio non servono semafori.
5. Dopo la seconda `wait`, solo il master esegue `shm_unlink`.

## Compilazione

```bash
make
```

## Esecuzione

```bash
./master
```

Durante l'esecuzione l'oggetto è visibile come file:

```bash
ls -l /dev/shm/so_es05_fork_exec
```

Lanciare `./writer` o `./reader` da soli, senza master, produce l'errore di `shm_open`: senza `O_CREAT` l'apertura fallisce se la risorsa non esiste.

## Osservazioni

- La differenza con l'esempio writer/reader della cartella `2_shmem`: lì i due eseguibili vanno avviati a mano da due terminali ed è il writer a creare e rimuovere la risorsa; qui il ciclo di vita è isolato in un processo dedicato, e si vede chiaramente chi crea, chi usa e chi rimuove.
- `close(fd)` subito dopo `mmap` (o dopo `ftruncate`, nel master) è legittimo: il mapping resta valido anche a descrittore chiuso, e l'oggetto esiste finché il nome non viene rimosso e nessuno lo mappa più.
- Se il master terminasse senza `shm_unlink`, l'oggetto resterebbe in `/dev/shm` anche a programmi finiti: è il motivo dello `shm_unlink` preventivo all'avvio.

## Domande di verifica

- Perché writer e reader non possono ereditare il mapping dal master, come accade ai figli di una semplice `fork`?
- Cosa succederebbe se anche il writer eseguisse `shm_unlink` alla fine?
- Perché il reader può aprire con `O_RDONLY` mentre il writer ha bisogno di `O_RDWR`?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
