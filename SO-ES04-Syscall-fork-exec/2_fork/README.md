## Creazione di un processo: la ``fork()``

La creazione di nuovi processi è gestita mediante la seguente chiamata di sistema:

```	c
pid_t fork(void);
```

La ``fork()`` crea una copia **esatta** (duplicazione) del processo chiamante. Il nuovo processo viene chiamato *figlio* (child), mentre il processo chiamante è detto *padre* (parent). Le aree dati globali, stack, heap e U-area sono copiate dal padre al figlio, e i due processi eseguono in spazi di memoria completamente separati. Pertanto:

- Il figlio eredita gli stessi valori delle variabili, i descrittori agli stessi file aperti, e anche il Program Counter (PC);
- Le modifiche apportate alle proprie variabili da uno dei due processi _non_ sono visibili all’altro.

Padre e figlio condivideranno l’area testo.

Siccome viene copiato anche il PC, processo padre e processo figlio riprenderanno l’esecuzione dal punto in cui è stata eseguita la ``fork()``. 

La ``fork()`` restituisce un valore intero che viene utilizzato per discriminare se si è nel contesto di esecuzione del processo padre o del processo figlio. In particolare (vedere figura):

- Se la ``fork()`` ha successo, il PID del processo *figlio* viene restituito nel contesto di esecuzione del processo *padre*, mentre viene restituito ``0`` nel contesto di esecuzione del processo *figlio*;
- Se la ``fork()`` fallisce, ad esempio perché non c'è abbastanza memoria RAM da allocare per il processo da creare, viene restituito il valore ``-1``.

<p align="center">
<img src="../images/fork_parent_child.png" width="500" > 
</p>


E' possibile approfondire il funzionamento della ``fork()`` analizzando il manuale (``man fork``).

Secondo quanto detto fin'ora, l'utilizzo corretto della ``fork()`` è il seguente:

```c
pid = fork();
 
// Controllo che non è fallita la chiamata
if (pid == -1) { 
	fprintf(stderr, "fork failed\n"); 
	exit(1); 
} 

// Scrivo il codice inerente al processo figlio
if (pid == 0) { 
	printf("This is the child\n"); 
	// Aggiungere altro
	exit(0); 
} 

// Scrivo il codice inerente al processo padre
if (pid > 0) { 
	printf("This is parent. The child is %d\n", pid); 
	// Aggiungere altro
	exit(0); 
}
```

### Creazione di *N* processi

Per creare *N* processi è necessario richiamare la system call ``fork()`` *N* volte, imponendo che il solo processo padre possa creare ulteriori processi figli!

La soluzione classica **sbagliata** è la seguente:

```c
int i, pid, N=3;
for (i=0; i<N; i++){
	pid = fork();
	printf("Sono il FIGLIO con PID: %d\n", getpid());
}
```

Assumendo ``N=3``, tale codice non creerà 3 processi figli, ma ogni volta che si richiamerà la ``fork()`` il processo figlio appena creato avrà il proprio flusso di controllo, e dipendentemente dal valore di ``i`` potrà creare altri processi figli a sua volta (questo è alla base del famoso attacco informatico [fork bomb](https://en.wikipedia.org/wiki/Fork_bomb))!

La soluzione corretta è:

```c
int i, pid, N=3;
for (i=0; i<N; i++){
	pid = fork();
	if (pid == 0){
		printf("Sono il FIGLIO con PID: %d\n", getpid());
		i=3;
	}
}
```

Analizzare il codice [main.c](main.c), provando ad eseguire il codice commentando e decommentando la [riga 13](main.c#L13), oppure confrontarlo con [fork_sbagliato.c](fork_sbagliato.c) (vedere gli esperimenti in fondo alla pagina).





## Attesa e terminazione di un processo

### ``wait()``

Per poter consentire ad un processo padre di raccogliere l’eventuale stato di terminazione dei figli è possibile utilizzare la system call ``wait()``. La firma della funzione è la seguente:

```c
pid_t wait(int *wstatus);
```

La system call ``wait()`` sospende l'esecuzione del processo chiamante fino a che uno dei suoi processi figli termina. Essa restituisce il PID del processo figlio che è terminato; se non esistono figli, o in caso di errore, viene restituito ``-1``.

Se ``wstatus`` non è ``NULL``, la funzione ``wait()`` memorizza nel valore intero puntato da tale variabile le informazioni sullo stato del processo. Questo valore può essere analizzato utilizzando diverse macro, come ad esempio:
- ``WIFEXITED(wstatus)`` che restituisce il valore booleano ``true`` se il processo figlio è terminato normalmente, attraverso la chiamata ad ``exit()`` o dal return dal ``main()``
- ``WEXITSTATUS(wstatus)`` che restituisce un valore intero che rappresenta l'exit status code utilizzato con la chiamata a exit() (e.g., exit(42) indica che WEXITSTATUS(wstatus) restituirà 42)

Inoltre, la variabile ``wstatus`` contiene (in forma codificata, da leggere con ``WEXITSTATUS``) il valore passato dal processo figlio alla system call ``exit()``.
Ci sono alcuni casi da considerare:

- Se un processo è terminato ma il suo genitore non ha ancora atteso (wait) la sua fine, il processo terminato viene definito processo **zombie**. In tal caso il kernel rilascia tutte le risorse di tale processo tranne il suo stato di terminazione, per dargli la possibilità di ricongiungersi con il padre (wait).

- Se il processo genitore termina prima dei suoi processi figli, attivi o zombie, tali processi sono detti **orfani**. In tal caso Unix assegna all’ID del processo padre il valore ``1``, cioè diventano figli del processo init (che non termina mai). In Linux un processo può anche dichiararsi *subreaper* e adottare gli orfani dei propri discendenti: su molti desktop lo fa ``systemd --user``, e in quel caso il nuovo padre non ha PID 1. I figli non sono consapevoli della terminazione del processo padre.

### ``exit()``

Un processo termina con la chiamata di sistema ``exit()``. La firma è la seguente:

```c
void exit(int status);
```

Quando la ``exit()`` viene invocata, uno stato di uscita con valore intero viene passato dal processo al kernel. Tale valore è disponibile al processo padre attraverso la chiamata di sistema ``wait()``. Per convenzione, lo stato di uscita ``0`` indica successo e un valore diverso da ``0`` indica un errore. Al padre arrivano solo gli 8 bit meno significativi del valore passato a ``exit()``, quindi i valori utili vanno da 0 a 255.

In particolare, lo stato di terminazione di un processo è un intero a 16 bit. Nel byte meno significativo vengono indicate informazioni relative a come il figlio è terminato, e più precisamente:

- ``0``: volontariamente;
- <span>&#8800;</span> ``0``: involontariamente (il valore esprime il segnale ricevuto).

Nel caso in cui il figlio termini volontariamente, il byte più significativo contiene lo stato di terminazione (il valore del parametro attuale passato alla ``exit()``, ovvero ``0``).  

<p align="center">
<img src="../images/wait_and_exit.png" width="300" > 

<img src="../images/fork_wait_exit.png" width="200" > 
</p>

## Esempi da provare

### 1. Compilare

```console
$ make
gcc -o fork_ex main.c
gcc -o copia copia.c
gcc -o fork_sbagliato fork_sbagliato.c
```

### 2. Una copia, non la stessa memoria

Il programma [``copia.c``](copia.c) ha una variabile globale ``x = 10``. Dopo la ``fork()`` il figlio la incrementa di 5, poi entrambi la stampano, insieme al suo indirizzo.

> **_Domanda:_** prima di eseguirlo: quale valore di ``x`` stamperà il padre?

```console
$ ./copia
[215923] prima della fork: x = 10
[215924] figlio: x = 15, indirizzo di x = 0x5cc1edc16010
[215923] padre:  x = 10, indirizzo di x = 0x5cc1edc16010
```

- Il padre stampa ``10``: il figlio ha modificato **la propria copia** di ``x``.
- L'indirizzo di ``x`` è **lo stesso** nei due processi, eppure i valori sono diversi. Gli indirizzi che vede un programma sono *virtuali*: ogni processo ha il proprio spazio di indirizzamento, e lo stesso indirizzo virtuale corrisponde, nei due processi, a due pagine fisiche diverse.
- ``wait(NULL)`` fa aspettare al padre la fine del figlio, così le stampe escono in ordine. La vediamo nella sezione successiva.

### 3. Creare *N* figli

Il programma [``main.c``](main.c) usa la soluzione corretta, con ``N = 3``.

> **_Domanda:_** quante volte verrà stampata la riga ``Istruzione successiva``?

```console
$ ./fork_ex
Sono il FIGLIO con PID: 215926
Istruzione successiva 
Istruzione successiva 
Sono il FIGLIO con PID: 215927
Istruzione successiva 
Sono il FIGLIO con PID: 215928
Istruzione successiva 
```

Quattro volte: una per il padre e una per ciascuno dei tre figli. Con ``i=3`` un figlio esce dal ciclo, ma poi continua a eseguire il codice che segue, come il padre.

> **_N.B.:_** il padre non aspetta i figli: se termina prima di loro, la shell ristampa il prompt e le righe dei figli compaiono dopo. Basta premere Invio per riavere il prompt.

### 4. La soluzione sbagliata

[``fork_sbagliato.c``](fork_sbagliato.c) è la "Soluzione 1" delle slide: ``fork()`` in un ciclo, senza distinguere padre e figlio. Alla fine ogni processo stampa il proprio PID e quello del padre, poi aspetta i propri figli.

```console
$ ./fork_sbagliato
Sono 215940, mio padre è 215921
Sono 215941, mio padre è 215940
Sono 215945, mio padre è 215941
Sono 215944, mio padre è 215940
Sono 215942, mio padre è 215940
Sono 215943, mio padre è 215941
Sono 215946, mio padre è 215942
Sono 215947, mio padre è 215943
```

Con ``N = 3`` ci sono 8 processi (2^3), non 4: a ogni giro del ciclo **tutti** i processi esistenti chiamano ``fork()``, e il loro numero raddoppia. Con i PID e i PID dei padri si può disegnare l'albero: ``215940`` ha tre figli, ``215941`` ne ha due, ``215942`` e ``215943`` uno ciascuno.

```console
$ ./fork_sbagliato 5 | wc -l
32
```

Con ``N = 5``, come nelle slide, i processi sono 32. Un ciclo infinito di ``fork()`` è la [fork bomb](https://en.wikipedia.org/wiki/Fork_bomb): per sicurezza il programma rifiuta valori di ``N`` maggiori di 10.

### 5. Per casa: una ``printf`` stampata due volte

```console
$ ./copia | cat
[215982] prima della fork: x = 10
[215984] figlio: x = 15, indirizzo di x = 0x5bee3c00d010
[215982] prima della fork: x = 10
[215982] padre:  x = 10, indirizzo di x = 0x5bee3c00d010
```

Se l'output va in una *pipe* (``|``) o in un file, la riga ``prima della fork`` compare **due volte**. Perché?

``printf`` non scrive subito: accumula il testo in un buffer, che sta nella memoria del processo. Quando l'output è il terminale il buffer viene svuotato a ogni ``\n``; quando è una pipe o un file, solo quando è pieno o quando il processo termina. Al momento della ``fork()`` il testo è ancora nel buffer, che viene copiato insieme al resto della memoria: così lo stampano sia il padre sia il figlio. Per evitarlo basta chiamare ``fflush(stdout)`` prima della ``fork()``.

Gli esempi su ``wait()`` ed ``exit()`` sono nella cartella [``2b_wait_exit``](../2b_wait_exit).
