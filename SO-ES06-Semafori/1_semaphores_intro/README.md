# Introduzione ai semafori POSIX

I due programmi mostrano le due modalità di condivisione dei semafori POSIX: un semaforo nominato, aperto tramite nome, e un semaforo anonimo collocato in una shared memory POSIX.

## Obiettivo didattico

Confrontare creazione, condivisione e distruzione di un semaforo nominato con quelle di un semaforo anonimo process-shared.

## Primitive e strutture utilizzate

- `sem_open`, `sem_close` e `sem_unlink` per il semaforo nominato.
- `sem_init` e `sem_destroy` per il semaforo anonimo.
- `sem_wait` e `sem_post` per bloccare e risvegliare il processo figlio.
- `shm_open`, `ftruncate` e `mmap` per ospitare il semaforo process-shared.

## Struttura dei file

- `named_sem.c`: crea un semaforo nominato con valore iniziale zero e sincronizza padre e figlio.
- `process_shared_sem.c`: colloca un `sem_t` anonimo in shared memory e lo inizializza con `pshared=1`.
- `Makefile`: genera `named_sem` e `process_shared_sem`.

## Funzionamento

1. In entrambi i programmi il semaforo è inizializzato a zero.
2. Dopo `fork`, il figlio invoca `sem_wait` e rimane bloccato.
3. Il padre stampa un messaggio e invoca `sem_post`, rendendo possibile la prosecuzione del figlio.
4. Nel caso nominato, padre e figlio possiedono un riferimento restituito da `sem_open`; nel caso anonimo, entrambi accedono allo stesso `sem_t` nel mapping condiviso.
5. Dopo `wait`, il padre esegue la procedura di distruzione appropriata.

## Compilazione

```bash
make
```

Il `Makefile` compila con `gcc` e in fase di collegamento include le librerie richieste dalle primitive POSIX impiegate.

## Esecuzione

```bash
./named_sem
./process_shared_sem
```

## Terminazione e cleanup

`named_sem` esegue `sem_close` e `sem_unlink`. `process_shared_sem` esegue `sem_destroy`, `munmap`, `close` e `shm_unlink` dopo la terminazione del figlio.

## Osservazioni

- Un semaforo anonimo con `pshared=0` sarebbe adatto alla sincronizzazione tra thread dello stesso processo, non tra processi distinti.
- Il semaforo deve rimanere in memoria condivisa per tutta la durata del suo utilizzo da parte dei processi.

## Domande di verifica

- Perché il valore iniziale zero blocca il figlio?
- Cosa cambierebbe inizializzando il semaforo a uno?
- Quando è preferibile un semaforo nominato rispetto a uno anonimo process-shared?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
