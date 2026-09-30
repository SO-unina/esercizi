# Una semplice shell

L'interprete dei comandi (*shell*) ripete sempre lo stesso ciclo:

1. stampa il prompt e legge una riga di comando;
2. divide la riga in parole: la prima è il nome del programma, le altre sono i suoi argomenti;
3. crea un figlio con ``fork()``; il figlio esegue il programma con ``execvp()``;
4. il padre aspetta con ``wait()`` che il figlio termini, poi ricomincia.

In questa cartella ci sono due soluzioni dell'esercizio proposto in [``3_exec``](../3_exec#esercizio-implementare-una-unix-shell):

- [``shell.c``](shell.c): la shell di base, che segue esattamente questo ciclo. La riga viene divisa in parole con ``strtok()``, e le parole finiscono nel vettore ``argv`` passato a ``execvp()``, terminato da ``NULL``;
- [``shell-background.c``](shell-background.c): come la precedente, ma se l'ultima parola è ``&`` il comando viene eseguito in *background*, cioè senza aspettarne la fine.

Per uscire da entrambe si scrive ``quit``.

## Esempi da provare

### 1. Compilare

```console
$ make
gcc -o shell shell.c
gcc -o shell-background shell-background.c
```

### 2. La shell di base

```console
$ ./shell

     ^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^

     --    Questo programma simula il funzionamento di una shell    --

     ^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^

                        * Per uscire digita quit *                    

my-shell>> ls -l
total 52
-rw-rw-r-- 1 studente studente   199 Sep 28 16:45 Makefile
-rwxrwxr-x 1 studente studente 16544 Sep 28 16:50 shell
-rwxrwxr-x 1 studente studente 16680 Sep 28 16:50 shell-background
-rw-rw-r-- 1 studente studente  3328 Sep 28 16:44 shell-background.c
-rw-rw-r-- 1 studente studente  2536 Sep 28 16:44 shell.c

my-shell>> ps -o pid,ppid,stat,comm
    PID    PPID STAT COMMAND
 216631  216630 Ss   bash
 216643  216631 S+   shell
 216645  216643 R+   ps
```

Il padre di ``ps`` ora è la nostra ``shell``, che a sua volta è figlia di ``bash``. ``shell`` è nello stato ``S``: è bloccata nella ``wait()``, in attesa che ``ps`` termini.

Se il comando non esiste, fallisce la ``execvp()``; se invece esiste ma termina con un errore, la shell lo scopre dallo stato di uscita:

```console
my-shell>> pippo
pippo: comando non trovato!
Il figlio ha terminato con stato di errore (1)

my-shell>> ls /non_esiste
ls: cannot access '/non_esiste': No such file or directory
Il figlio ha terminato con stato di errore (2)
```

> **_Domanda:_** e se proviamo a cambiare cartella?

```console
my-shell>> cd /tmp
cd: comando non trovato!
Il figlio ha terminato con stato di errore (1)

my-shell>> quit

                           * Arrivederci!! *                    
```

``cd`` non è un programma, ma un comando **interno** (*built-in*) della shell. E non potrebbe essere altrimenti: la cartella corrente è un attributo del processo. Se ``cd`` fosse eseguito da un figlio, cambierebbe la cartella del figlio, che subito dopo termina, mentre la shell resterebbe dov'era. Per questo ``bash`` esegue ``cd`` direttamente, senza ``fork()``, chiamando la system call ``chdir()``. Lo stesso vale per ``exit``.

### 3. Comandi in background

```console
$ ./shell-background
...
my-shell>> sleep 30 &

my-shell>> ps -o pid,ppid,stat,comm
    PID    PPID STAT COMMAND
 213540  213530 Ss   bash
 213550  213540 S+   shell-backgroun
 213552       1 S    sleep
 213565  213550 R+   ps

my-shell>> quit
```

Il prompt torna subito, e ``sleep`` continua a girare. Ma il suo padre non è la nostra shell: è ``init`` (PID 1), oppure ``systemd --user`` su alcuni desktop.

Per non dover chiamare ``wait()`` sui comandi in background, [``shell-background.c``](shell-background.c) usa il trucco della **doppia fork**: il figlio crea a sua volta un *nipote*, che esegue il comando, e termina subito. La shell aspetta solo il figlio, che termina subito; il nipote resta orfano e viene adottato da ``init``, che chiamerà ``wait()`` per lui. Così nessun comando in background resta zombie (si veda l'esperimento sugli orfani in [``2b_wait_exit``](../2b_wait_exit)).

Il nipote, prima della ``exec``, collega il proprio standard input a ``/dev/null`` (con ``open()`` e ``dup2()``), così un comando in background non ruba l'input da tastiera alla shell.

### 4. Esercizio

Partendo da [``shell.c``](shell.c):

1. aggiungete i comandi interni ``cd`` (con la system call ``chdir()``, vedi ``man 2 chdir``) ed ``exit``;
2. mostrate nel prompt lo stato di uscita dell'ultimo comando, come fa ``bash`` con ``$?``;
3. fate in modo che ``sleep 5&`` (senza spazio prima di ``&``) venga riconosciuto come comando in background.
