## Esecuzione di un programma: la famiglia ``exec()``

L'unico modo in cui un programma può essere eseguito da Unix è che il processo corrente invochi la chiamata di sistema appartenente alla famiglia ``exec()`` (eccetto per il processo init). Il nuovo programma viene eseguito nel contesto del processo chiamante, cioè il PID non cambia. Se non ci sono errori, la chiamata a ``exec()`` non fa ritorno e il controllo passa al nuovo programma, mentre ritorna al chiamante solo se si verifica un errore.

Il processo dopo la chiamata ad ``exec()``:

- Mantiene la stessa *process structure*;
- Ha codice, dati globali, stack e heap nuovi;
- Riferisce una nuova *text structure*;
- Mantiene *user area* (a parte PC e informazioni legate al codice) e stack del kernel.

<p align="center">
<img src="../images/fork_exec_wait_exit.png" width="200" > 
</p>


L'``exec()`` è anche nota come "sostituzione di codice". Questo significa che dopo la chiamata, il processo che esegue è lo stesso, ma esso esegue un programma differente.

Le possibili implementazioni della ``exec()`` sono due:

- Sovra-scrittura del segmento di memoria corrente con nuovi valori;
- Allocazione di nuovi segmenti di memoria, inizializzazione di questi con i valori del nuovo processo e deallocazione dei segmenti *vecchi*.

Nel 99% dei casi, dopo una ``fork()`` viene eseguita una ``exec()``. L’operazione di copia della memoria tra processo padre e processo figlio è nella maggior parte dei casi sprecata, perché il figlio la sostituisce subito con quella del nuovo programma.

In BSD e in Linux è disponibile la ``vfork()``, una system call che crea un processo senza copiare l’immagine dal padre al figlio. Spesso tale chiamata di sistema è detta lightweight ``fork()``, ed obbliga il processo figlio ad invocare subito la ``exec()`` (o ``_exit()``): fino ad allora il padre resta sospeso.

In Linux viene adottato un meccanismo diverso chiamato **copy-on-write**. Inizialmente il processo figlio condivide la memoria del processo padre, configurata come *read-only*. Al primo tentativo di modifica di una pagina, da parte del figlio o del padre, il kernel provvederà a copiare quella pagina (e solo quella). Un esempio è nella cartella [``3b_copy_on_write``](../3b_copy_on_write).

<p align="center">
<img src="../images/copy-on-write.png" width="500" > 
</p>

Disaccoppiare la ``fork()`` dalla ``exec()`` dà la possibilità al programmatore di gestire il processo figlio solo a valle della sua creazione, in maniera completamente indipendente dal processo padre. E.g.:

```c
int pid = fork();	// crea il figlio
if(pid == 0) {		// il figlio continua qui

	// Op. qualsiasi (libera memoria, chiudi connessioni, etc.)
	
	execl("program", arg0, arg1, arg2, …);
}
```


### Famiglia delle ``exec()``

Esistono varie versioni della ``exec()``:

```c
//Percorso completo dell’eseguibile; parametri tramite lista
int execl(char *path, char *arg0, .., char *argn, (char *) 0);

//Nome dell’eseguibile (cercato nelle cartelle della variabile PATH); parametri tramite lista
int execlp(char *nomefile, char *arg0, .., char *argn, (char *) 0);
```

In ``execl()`` ed ``execlp()`` notare come si usi il puntatore nullo (``(char *) 0``, oppure ``NULL``) per indicare la fine dei parametri da passare al comando. Non va scritto ``0`` da solo: in una funzione con un numero variabile di argomenti verrebbe passato come ``int`` (4 byte) e non come puntatore (8 byte), e ``gcc -Wall`` lo segnala con il warning *missing sentinel*.

Altre versioni della ``exec()`` sono:

```c
//Percorso completo dell’eseguibile; parametri tramite array
int execv(const char *path, char *const argv[]);

//Nome dell’eseguibile (cercato nelle cartelle della variabile PATH); parametri tramite array
int execvp(const char *nomefile, char *const argv[]);
```

Ad esempio, per poter invocare un comando chiamato ``program``, basta generare un processo figlio e richiamare una delle system call appartenenti alla famiglia ``exec()``:

```c
pid = fork();
if (pid == 0) {
	// codice figlio
	...
	if (execlp("program",...) < 0){
   		perror("exec fallita");
	   exit(1);
	}

} else if (pid < 0){
	perror("fork fallita");
}
// Il padre continua da questo punto in poi.
// Generalmente, effettua una wait() sul figlio.
```

### Esecuzione del comando ``ls -l`` tramite ``exec()``

Analizzare il programma [main_ls.c](main_ls.c) che fa uso della funzione ``execl()`` per poter eseguire il comando ``ls -l``. Notare come nella chiamata a ``execl()`` abbiamo: 

```c
execl("/bin/ls", "ls", "-l", NULL);
```
Ovvero:

- Il primo parametro è il path del comando ``ls`` (includendo il suo nome);
- Il secondo parametro è il nome del comando (``ls``);
- Il terzo parametro è il flag ``-l``;
- L'ultimo parametro sia ``NULL``.



Provando ad eseguire il programma [main_ls.c](main_ls.c), otterremmo una cosa del genere:

```console
$ gcc -o main_ls main_ls.c
$ ./main_ls
ESECUZIONE del comando 'ls -l'...
total 40
-rw-r--r-- 1 so  so  1930 Oct 19 13:05 README.md
-rwxr-xr-x 1 so  so  8708 Oct 19 13:07 main_ls
-rw-r--r-- 1 so  so   592 Oct 19 13:07 main_ls.c
Il figlio 77657 ha terminato l'esecuzione con stato: 0
```

### I parametri ``argc`` e ``argv`` del ``main()``

Nell'utilizzo di ``exec()``, è possibile sfruttare i cosiddetti parametri passati dalla cosiddetta *command line* al ``main()``. In particolare, è possibile utilizzare la seguente firma per la funzione ``main()``:

```c
int main(int argc, char *argv[]){
	...
	return 0;
}
```

dove ``argc`` indica il numero di stringhe puntate dal parametro ``*argv[]`` che invece sta ad indicare il puntatore all'array di stringhe in ingresso: ``argv[0]`` indica la prima stringa, ``argv[1]`` la seconda, e così via.

Analizziamo il codice in [main_cp.c](main_cp.c). Questo programma invoca il comando ``/bin/cp`` su due parametri che sono passati tramite linea di comando al ``main()``:

```c
execl("/bin/cp", "cp", argv[1], argv[2], NULL);
```

Notare come l'indice dei parametri di ingresso parta da ``1`` e non da ``0``; questo perché la stringa ``argv[0]`` identifica il nome dell'eseguibile che state avviando (provate a stampare la variabile ``argv[0]``). Il programma, prima di tutto, controlla che ``argc`` valga 3: il nome del programma più i due file.
Compilando il programma, e provandolo ad eseguire, otterremo come di seguito:

```console
$ gcc -o main_cp main_cp.c
$ ./main_cp main_cp.c main_cp.c.backup
Sono il processo padre, con PID 87487
Sono il processo figlio, con PID 87488
Command line passata al main():
argv[0]: ./main_cp
argv[1]: main_cp.c
argv[2]: main_cp.c.backup
Attendo 3 secondi
.
.
.
Copia effettuata con successo!
Il processo padre termina
```

## Esempi da provare

### 1. Compilare

```console
$ make
gcc -o main_cp main_cp.c
gcc -o main_ls main_ls.c
gcc -o sostituzione sostituzione.c
```

### 2. ``exec()`` senza ``fork()``: il PID non cambia

Il programma [``sostituzione.c``](sostituzione.c) non crea nessun figlio: stampa il proprio PID e poi chiama ``execvp()`` con il comando ricevuto sulla riga di comando. Il vettore degli argomenti è ``&argv[1]``, cioè ``argv`` senza il primo elemento (anche questo vettore termina con ``NULL``, perché ``argv[argc]`` vale sempre ``NULL``).

> **_Domanda:_** eseguiamo ``ps`` al posto di ``sostituzione``. Quale PID avrà il processo ``ps``?

```console
$ ./sostituzione ps -o pid,ppid,comm
Sono il processo 215986, mio padre è 215921: ora eseguo ps
    PID    PPID COMMAND
 215921  215920 bash
 215986  215921 ps
```

Lo stesso, 215986: è lo stesso processo, che ora esegue il programma ``ps``. Nell'elenco non c'è nessun processo ``sostituzione``: il suo codice è stato sostituito.

Se il comando non esiste, la ``exec`` fallisce e ritorna: solo in questo caso viene eseguita l'istruzione successiva.

```console
$ ./sostituzione pippo
Sono il processo 215987, mio padre è 215921: ora eseguo pippo
exec fallita: No such file or directory
```

``perror()`` stampa il messaggio passato come argomento, seguito dalla descrizione dell'errore contenuto in ``errno``.

### 3. ``fork()`` + ``exec()``

```console
$ ./main_ls
ESECUZIONE del comando 'ls -l'...
total 76
-rw-rw-r-- 1 studente studente   265 Sep 28 16:43 Makefile
-rw-rw-r-- 1 studente studente  6753 Sep 28 16:15 README.md
-rwxrwxr-x 1 studente studente 16448 Sep 28 16:49 main_cp
-rw-rw-r-- 1 studente studente  1637 Sep 28 16:43 main_cp.c
-rwxrwxr-x 1 studente studente 16312 Sep 28 16:49 main_ls
-rw-rw-r-- 1 studente studente   597 Sep 28 16:43 main_ls.c
-rwxrwxr-x 1 studente studente 16296 Sep 28 16:49 sostituzione
-rw-rw-r-- 1 studente studente   864 Sep 28 16:43 sostituzione.c
Il figlio 215989 ha terminato l'esecuzione con stato: 0

```

Il figlio diventa ``ls``, il padre lo aspetta con ``wait()``. [``main_ls.c``](main_ls.c) legge lo stato "a mano", come nella slide "Wait e Exit": ``(char)st`` è il byte meno significativo (0 se il figlio ha chiamato ``exit()``), ``st>>8`` quello più significativo (il valore passato a ``exit()``). Le macro ``WIFEXITED`` e ``WEXITSTATUS`` fanno la stessa cosa in modo portabile.

### 4. Passare argomenti al programma: ``main_cp``

```console
$ ./main_cp main_cp.c copia.c
Sono il processo padre, con PID 215990
Sono il processo figlio, con PID 215991
Command line passata al main():
argv[0]: ./main_cp
argv[1]: main_cp.c
argv[2]: copia.c
Attendo 3 secondi
.
.
.
Copia effettuata con successo!
Il processo padre termina
$ diff main_cp.c copia.c && echo "file identici"
file identici
```

Il padre scopre se la copia è riuscita dallo **stato di uscita** di ``cp``: 0 se è andato tutto bene, un valore diverso da 0 in caso di errore.

```console
$ ./main_cp non_esiste.c copia.c
Sono il processo padre, con PID 216023
Sono il processo figlio, con PID 216024
Command line passata al main():
argv[0]: ./main_cp
argv[1]: non_esiste.c
argv[2]: copia.c
Attendo 3 secondi
.
.
.
cp: cannot stat 'non_esiste.c': No such file or directory
La copia non è andata a buon fine
Il processo padre termina
$ rm copia.c
```

Qui la ``exec`` è riuscita (``cp`` è partito), ma ``cp`` è terminato con ``exit(1)``.

### 5. Chi fa davvero che cosa: ``strace``

``strace`` mostra le system call eseguite da un programma (se non è installato: ``sudo apt install strace``). Con ``-f`` segue anche i processi figli, con ``-e trace=process`` mostra solo quelle che riguardano i processi. Lo standard output di ``main_ls`` lo buttiamo in ``/dev/null``: ``strace`` scrive sullo standard error, quindi vediamo solo le sue righe.

```console
$ strace -f -qq -e trace=process ./main_ls > /dev/null
execve("./main_ls", ["./main_ls"], 0x7ffd50489180 /* 8 vars */) = 0
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7a776fd30a10) = 216058
[pid 216057] wait4(-1,  <unfinished ...>
[pid 216058] execve("/bin/ls", ["ls", "-l"], 0x7ffe23cb8d68 /* 8 vars */) = 0
[pid 216058] exit_group(0)              = ?
<... wait4 resumed>[{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 216058
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=216058, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
exit_group(0)                           = ?
```

Le funzioni che usiamo in C sono funzioni della libreria C, che chiamano le system call del kernel. I nomi non sempre coincidono:

| Nel programma C | System call eseguita |
|---|---|
| ``fork()`` | ``clone()`` |
| ``execl()``, ``execlp()``, ``execv()``, ``execvp()`` | ``execve()`` |
| ``wait()`` | ``wait4()`` |
| ``exit()`` (o ``return`` dal ``main``) | ``exit_group()`` |
| ``sleep()`` | ``clock_nanosleep()`` |
| ``printf()`` | ``write()`` |
| ``getpid()`` | ``getpid()`` |

La prima riga è la ``execve`` con cui la shell ha avviato ``main_ls``. ``SIGCHLD`` è il segnale con cui il kernel avvisa il padre che un figlio è terminato.

Qualche dettaglio:

- ``clone`` è la system call più generale per creare un processo: con altre opzioni crea anche i thread. La ``fork()`` della libreria la usa (al posto della vecchia system call ``fork``) con opzioni che danno lo stesso risultato;
- ``wait()`` e ``waitpid()`` sono due funzioni, ma nel kernel c'è la sola ``wait4``;
- ``exit()`` è una funzione di libreria: prima svuota i buffer di ``printf`` e chiude i file della libreria, poi esegue ``exit_group``, che termina tutti i thread del processo. ``_exit()``, usata negli esempi dopo una ``exec`` fallita, esegue subito la system call;
- il manuale distingue i due livelli: la sezione 2 descrive le system call (``man 2 wait4``), la sezione 3 le funzioni di libreria (``man 3 exit``).

Solo ``execve`` è una system call: le altre varianti della ``exec`` sono funzioni di libreria. Con la ``p`` finale (``execlp``, ``execvp``) è la libreria che cerca il comando nelle cartelle della variabile ``PATH``, provando una ``execve`` per ogni cartella, finché una non riesce:

```console
$ echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
$ strace -qq -e trace=execve ./sostituzione ls > /dev/null
execve("./sostituzione", ["./sostituzione", "ls"], 0x7fff2f53bdf0 /* 8 vars */) = 0
execve("/usr/local/sbin/ls", ["ls"], 0x7fffa602c980 /* 8 vars */) = -1 ENOENT (No such file or directory)
execve("/usr/local/bin/ls", ["ls"], 0x7fffa602c980 /* 8 vars */) = -1 ENOENT (No such file or directory)
execve("/usr/sbin/ls", ["ls"], 0x7fffa602c980 /* 8 vars */) = -1 ENOENT (No such file or directory)
execve("/usr/bin/ls", ["ls"], 0x7fffa602c980 /* 8 vars */) = 0
```

I manuali: ``man 2 execve`` per la system call, ``man 3 exec`` per le funzioni di libreria.

### 6. Una ``printf`` che sparisce

```console
$ ./main_ls | head -3
total 76
-rw-rw-r-- 1 studente studente   265 Sep 28 16:43 Makefile
-rw-rw-r-- 1 studente studente  6753 Sep 28 16:15 README.md
```

Se l'output va in una pipe, la riga ``ESECUZIONE del comando 'ls -l'...`` non compare più. Il motivo è lo stesso dell'esperimento 5 di [``2_fork``](../2_fork#5-per-casa-una-printf-stampata-due-volte): verso una pipe ``printf`` accumula il testo in un buffer, nella memoria del processo, e la ``exec`` sostituisce la memoria prima che il buffer venga svuotato. Per questo [``sostituzione.c``](sostituzione.c) chiama ``fflush(stdout)`` prima della ``exec``.

## Esercizio. Implementare una Unix shell

L’interprete dei comandi (*shell*) implementa le seguenti funzionalità:

- Legge la linea di comando;
- Estrae la prima parola, assumendo che sia il nome di un programma;
- Cerca il programma in questione e lo lancia in esecuzione (mediante fork ed exec), passandogli come argomenti le altre parole presenti sulla linea di comando;
- Attende che il figlio termini, prima di presentare nuovamente il prompt d’utente.

Utilizzare le system call viste fin'ora per implementare una Unix shell di base. Una possibile soluzione è nella cartella [``4_shell``](../4_shell).






