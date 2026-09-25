# GNU Debugger: ``gdb``

Il debugger è un programma usato per ispezionare un altro programma a tempo di esecuzione. In particolare, in Linux viene utilizzato ``gdb`` per programmi scritti in C, C++, Fortran, e Modula-2.

Il debugger permette di:

- Eseguire step by step linee di codice
- Ispezionare parti specifiche del codice
- Analizzare variabili e registri
- Analizzare locazioni di memoria

Per avviare il debugger:

```console
$ gdb --args nome_file_eseguibile [argomenti...]
```

Il file eseguibile per cui si vuole effettuare il debug con ``gdb`` deve essere compilato con l'opzione ``-g``, il quale istruisce il compilatore a generare le informazioni di debug.

### Comandi generali

- ``file``: seleziona il file da debuggare
- ``run(r)``: esegue il file 
- ``attach <npid>``: permette a ``gdb`` di attaccarsi al processo con pid npid 
- ``kill``: uccide il processo per cui stiamo facendone il debug
- ``quit``: esce da ``gdb``

### Esecuzione step by step

- ``c(ontinue)``: continua l’esecuzione fino al prossimo stop
- ``s(tep)``: esegue una linea di codice, entrando nella chiamata a funzione se la linea è una call.
- ``n(ext)``: esegue una linea di codice, senza entrare nella chiamata a funzione se la linea è una call.
- ``finish``: termina l’esecuzione di una funzione
 
### Breakpoints

- ``b(reak) <pos>``: imposta un breakpoint in una posizione, indicata come numero di riga (``b 12``), nome di funzione (``b main``) o ``file:riga`` (``b media_voti.c:16``)
- ``watch <expr>``: trigger quando *expr* è letto o scritto
- ``info break``: lista di tutti i breakpoints
- ``clear <pos>``: elimina un breakpoint alla posizione pos
- ``delete <num>``: elimina un breakpoint usando il numero
 
### Comandi per ricercare e stampare

- ``list <pos>``: stampa la linea di codice nella posizione *pos*
- ``search <regexp>``: ricerca i punti del codice sorgente che soddisfano l’espressione regolare regexp
- ``backtrace``: stampa lo stacktrace fino a quel punto
- ``info <var,fun>``: stampa informazioni su variabili e funzioni
- ``print <expr>``: stampa la valutazione dell’espressione *expr*

### Comandi per modificare dati e control path

- ``set <name> <expr>``: cambia il valore di una variabile
- ``return <expr>``: forza il valore di ritorno ad *expr* dalla funzione corrente
- ``jump <pos>``: fa saltare l’esecuzione a *pos*
 

### Altri comandi utili

- ``display <expr>``: stampa *expr* automaticamente a ogni passo
- ``info locals`` / ``info args``: variabili locali e argomenti della funzione corrente
- ``print *vettore@n``: stampa ``n`` elementi a partire dal puntatore ``vettore``
- ``until <riga>``: continua fino alla riga indicata (utile per uscire da un ciclo)
- ``frame <n>``, ``up``, ``down``: si sposta tra i frame dello stack mostrati da ``backtrace``
- ``Ctrl+X A`` (oppure ``gdb -tui``): attiva/disattiva la *Text User Interface*, che mostra il sorgente mentre si esegue

## Esempi

Compilare tutti gli esempi con ``make``. Il [``Makefile``](Makefile) usa le opzioni ``-g`` (informazioni di debug) e ``-O0`` (nessuna ottimizzazione: con ``-O2`` molte variabili risulterebbero ``<optimized out>``).

Le informazioni di debug sono sezioni in più dentro il file ELF:

```console
$ objdump -h media_voti | grep debug
 27 .debug_aranges 00000030  0000000000000000  0000000000000000  0000303d  2**0
 28 .debug_info   00000158  0000000000000000  0000000000000000  0000306d  2**0
 29 .debug_abbrev 000000ef  0000000000000000  0000000000000000  000031c5  2**0
 30 .debug_line   00000091  0000000000000000  0000000000000000  000032b4  2**0
 31 .debug_str    00000104  0000000000000000  0000000000000000  00003345  2**0
 32 .debug_line_str 000000bd  0000000000000000  0000000000000000  00003449  2**0
```

``.debug_line`` contiene la corrispondenza tra istruzioni e righe del sorgente, ``.debug_info`` i nomi e i tipi di variabili e funzioni: è così che ``gdb`` può mostrare il codice C e le variabili per nome.

### 1. ``media_voti``: un bug logico

```console
$ ./media_voti
Media dei voti: 19.00           # con 30, 18, 27, 24, 28 dovrebbe essere 25.40!
```

Il programma non va in crash: semplicemente calcola un risultato sbagliato, che è il tipo di bug più subdolo.

> **_Domanda:_** prima di usare ``gdb``, leggete la funzione ``calcola_media`` in [``media_voti.c``](media_voti.c): quanti bug vedete?

Una possibile sessione di debug:

```console
$ gdb ./media_voti
(gdb) break calcola_media
(gdb) run
Breakpoint 1, calcola_media (voti=0x7fffffffd4b0, n=5) at media_voti.c:9
(gdb) print *voti@n
$1 = {30, 18, 27, 24, 28}       # i dati in ingresso sono corretti
(gdb) next
(gdb) next
(gdb) print i
$2 = 1                          # primo bug: il ciclo parte da 1 e salta il 30!
(gdb) set var i = 0             # correggiamo "al volo" senza ricompilare
(gdb) until 16
(gdb) print somma
$3 = 127                        # ora la somma e' giusta...
(gdb) print somma / n
$4 = 25                         # ...ma la divisione tra interi tronca
(gdb) print (double) somma / n
$5 = 25.399999999999999         # secondo bug trovato
(gdb) finish
Value returned is $6 = 25
```

``finish`` completa la funzione corrente e mostra il valore restituito; ``continue`` fa proseguire il programma fino alla fine.

#### ``step`` e ``next``

```console
$ gdb ./media_voti
(gdb) break 23                  # la riga che chiama calcola_media
(gdb) run
23		double media = calcola_media(voti, NUM_ESAMI);
(gdb) step                      # entra nella funzione chiamata
calcola_media (voti=0x7fffffffd490, n=5) at media_voti.c:9
9		int somma = 0;
(gdb) run                       # ricomincia da capo (confermare con y)
23		double media = calcola_media(voti, NUM_ESAMI);
(gdb) next                      # esegue tutta la funzione in un passo solo
25		printf("Media dei voti: %.2f\n", media);
```

``step`` entra nella funzione chiamata; ``next`` la esegue per intero e si ferma alla riga successiva.

#### Watchpoint: fermarsi quando una variabile cambia

```console
$ gdb ./media_voti
(gdb) break calcola_media
(gdb) run
(gdb) watch somma
Hardware watchpoint 2: somma
(gdb) continue
Old value = 0
New value = 18                  # il primo voto sommato è 18, non 30!
(gdb) continue
Old value = 18
New value = 45
(gdb) info break
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000555555555178 in calcola_media at media_voti.c:9
2       hw watchpoint  keep y                      somma
(gdb) delete 2
```

Un **breakpoint** ferma il programma quando l'esecuzione arriva in un punto del codice; un **watchpoint** quando cambia un dato. Già il primo valore mostra il primo bug: il 30 non viene mai sommato. Quando la funzione termina, ``gdb`` cancella da solo il watchpoint, perché ``somma`` non esiste più. ``info break`` elenca breakpoint e watchpoint con il loro numero, che serve a ``delete``.

#### Breakpoint condizionali

```console
$ gdb ./media_voti
(gdb) break 13 if i == 3
(gdb) run
13			somma += voti[i];
(gdb) print i
$1 = 3
(gdb) print voti[i]
$2 = 24
```

Il programma si ferma alla riga 13 solo quando ``i`` vale 3: utilissimo nei cicli, quando il problema compare alla millesima iterazione.

### 2. ``crash``: segmentation fault

```console
$ ./crash
Esposito ha preso 30
Rossi ha preso 28
Segmentation fault (core dumped)
```

```console
$ gdb ./crash
(gdb) run
Program received signal SIGSEGV, Segmentation fault.
0x0000555555555207 in stampa_voto (cognome=0x555555556023 "Verdi") at crash.c:31
31		printf("%s ha preso %d\n", s->cognome, s->voto);
(gdb) backtrace
#0  0x0000555555555207 in stampa_voto (cognome=0x555555556023 "Verdi") at crash.c:31
#1  0x000055555555525d in main () at crash.c:37
(gdb) print s
$1 = (studente_t *) 0x0          # cerca() ha restituito NULL
(gdb) frame 1                    # sale nel frame di chi ha chiamato stampa_voto
#1  0x000055555555525d in main () at crash.c:37
37		stampa_voto("Verdi");		// non e' nel registro!
(gdb) list
```

Il crash è in ``stampa_voto``, chiamata da ``main`` alla riga 37 con ``cognome = "Verdi"``. ``s`` vale ``0x0``: ``cerca()`` restituisce ``NULL`` quando lo studente non è nel registro, e nessuno controlla il valore restituito.

**Analisi post-mortem (core dump)**: il programma è già terminato, ma il kernel ha salvato l'immagine della sua memoria al momento del crash. A seconda della configurazione del sistema:

```console
$ coredumpctl debug crash                 # sistemi con systemd-coredump
$ ulimit -c unlimited; ./crash; gdb ./crash core   # file "core" nella cartella corrente
```

### 3. ``cassaforte``

Il programma [``cassaforte.c``](cassaforte.c) chiede un PIN che non conoscete. Riuscite ad aprire la cassaforte usando solo ``gdb``?

**Come funziona il programma**

- ``main`` legge un numero con ``scanf`` e lo passa a ``verifica(tentativo)`` (riga 34);
- ``verifica`` calcola il PIN vero con ``genera_pin()`` (riga 17) e restituisce 1 se il tentativo coincide (riga 19);
- se ``verifica`` restituisce 1, ``main`` chiama ``apri_cassaforte()`` (riga 35), che stampa "CASSAFORTE APERTA!"; altrimenti stampa "PIN errato".

Il PIN non è scritto nel codice: ``genera_pin()`` lo calcola a runtime con un ciclo, quindi non compare "in chiaro" nell'eseguibile (provate ``strings cassaforte``).

> **_Suggerimento:_** ``info functions`` elenca le funzioni definite nel programma.

**Quattro modi per aprirla** (prima di leggerli, provate da soli!)

Lanciate ``gdb ./cassaforte``; dopo ``run`` il programma chiede il PIN: inseritene uno a caso. Ogni modo usa uno dei comandi per ispezionare o modificare il programma.

1. **Leggere il segreto** (``print``): ``break verifica``, ``run``, ``next``, ``print pin``. Il ``next`` esegue la riga 17, cioè il calcolo del PIN, e ``print pin`` lo mostra: basta rilanciare il programma e inserirlo. In alternativa, ``print genera_pin()``: ``gdb`` può chiamare direttamente una funzione del programma.
2. **Modificare un dato** (``set var``): ``break 19``, ``run``, ``set var tentativo = pin``, ``continue``. Alla riga 19 il PIN è già stato calcolato, ma il confronto non è ancora avvenuto: rendiamo il tentativo uguale al PIN, e il confronto dà "vero".
3. **Forzare il valore di ritorno** (``return``): ``break verifica``, ``run``, ``return 1`` (confermate con ``y``), ``continue``. La funzione termina subito restituendo 1, senza nemmeno calcolare il PIN: ``main`` crede che il PIN fosse giusto.
4. **Saltare il controllo** (``jump``): ``break 34``, ``run``, ``jump 35``. Ci fermiamo in ``main`` prima dell'``if`` e riprendiamo l'esecuzione direttamente dalla riga 35: il controllo non viene mai eseguito.

Il primo modo legge lo stato del programma, il secondo cambia un dato, il terzo cambia il risultato di una funzione, il quarto cambia il flusso di esecuzione.

### 4. ``bloccato``: attaccarsi a un processo in esecuzione

Il conto alla rovescia non termina mai (``secondi`` passa da 1 a -1 senza mai valere 0). Lanciatelo in un terminale:

```console
$ ./bloccato
Conto alla rovescia (PID 53777)
9... 7... 5... 3... 1... -1... -3...
```

e, da un secondo terminale, attaccatevi al processo con il suo PID:

```console
$ sudo gdb -p 53777
(gdb) backtrace                # il processo e' fermo dentro sleep(), chiamata da main
(gdb) frame 3                  # il numero del frame di main mostrato da backtrace
(gdb) print secondi
$1 = -5
(gdb) set var secondi = 0
(gdb) continue                 # nel primo terminale compare "Decollo!"
```

> **_N.B.:_** Su Ubuntu, per motivi di sicurezza, un utente normale può fare il debug solo dei processi **figli** di ``gdb`` (si veda ``/proc/sys/kernel/yama/ptrace_scope``). Per questo serve ``sudo``. In alternativa, si può lanciare il programma dentro ``gdb`` (``gdb ./bloccato``, ``run``) e interromperlo con ``Ctrl+C``.
