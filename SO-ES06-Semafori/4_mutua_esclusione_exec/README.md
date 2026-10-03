# Mutua esclusione tra eseguibili distinti con semaforo nominato

Il programma `start` crea un semaforo nominato e lancia con `fork` + `exec` due istanze dell'eseguibile `lavoratore`, che si contendono una sezione critica riaprendo il semaforo per nome.

## Obiettivo didattico

Mostrare il caso in cui il semaforo nominato è necessario: la `exec` sostituisce lo spazio di indirizzamento del nuovo programma e distrugge ogni mapping ereditato dalla `fork`, quindi un semaforo anonimo collocato in shared memory non sarebbe più raggiungibile. Le risorse nominate, invece, possono essere riaperte per nome da qualunque eseguibile.

## Primitive e strutture utilizzate

- `sem_open` con `O_CREAT | O_EXCL` e valore iniziale 1 in `start`; `sem_open(SEM_NAME, 0)` nei lavoratori, che richiede che il semaforo esista già.
- `fork` + `execl` per lanciare i due eseguibili.
- `sem_wait` e `sem_post` per la mutua esclusione; `sem_close` in ogni processo e `sem_unlink` nel creatore.

## Struttura dei file

- `header.h`: definisce il nome `SEM_NAME` e il numero di cicli.
- `start.c`: crea il semaforo, lancia i lavoratori "A" e "B", attende e rimuove il nome.
- `lavoratore.c`: riapre il semaforo per nome ed esegue i cicli di ingresso/uscita dalla sezione critica.
- `Makefile`: genera `start` e `lavoratore`.

## Funzionamento

1. `start` rimuove un eventuale semaforo residuo (`sem_unlink` preventivo) e lo crea con valore 1.
2. Ogni figlio esegue `execl("./lavoratore", ...)`: da quel momento il processo è un programma nuovo, senza alcun riferimento ereditato al semaforo.
3. Ogni lavoratore esegue `sem_open(SEM_NAME, 0)` e ritrova il semaforo tramite il nome.
4. Nei cicli, le stampe mostrano l'attesa, l'ingresso e l'uscita dalla sezione critica (lo `sleep` rende visibile l'esclusione).
5. Dopo le `wait`, `start` esegue `sem_close` e `sem_unlink`.

## Compilazione

```bash
make
```

## Esecuzione

```bash
./start
```

Durante l'esecuzione il semaforo è visibile come file in `/dev/shm`:

```bash
ls -l /dev/shm/sem.so_es06_mutex_exec
```

Lanciare `./lavoratore A` da solo, senza `start`, produce l'errore di `sem_open`: il semaforo non esiste finché il creatore non lo apre con `O_CREAT`.

## Osservazioni

- Il semaforo garantisce la mutua esclusione, non l'equità: lo stesso lavoratore può rientrare più volte di seguito mentre l'altro attende, perché chi rilascia e richiede subito il semaforo è già in esecuzione sulla CPU.
- I lavoratori aprono il semaforo senza `O_CREAT`: l'errore in assenza del creatore rende esplicita la dipendenza dall'ordine di avvio.

## Domande di verifica

- Perché un semaforo anonimo in shared memory non sopravvive alla `exec`?
- Cosa succede ai descrittori di file durante la `exec`? E ai mapping di memoria?
- Chi deve eseguire `sem_unlink`, e cosa accade ai processi che hanno ancora il semaforo aperto?

Per rimuovere gli eseguibili e i file oggetto:

```bash
make clean
```
