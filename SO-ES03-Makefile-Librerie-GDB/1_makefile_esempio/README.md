# Esercizio 1. Makefile

In questo esempio si vuole mostrare come è possibile scrivere un **Makefile** ed utilizzare il comando ``make``.

Il comando ``make`` è un'utility utilizzata specialmente per semplificare la compilazione separata di programmi composti da più file sorgente e librerie.
Digitando ``make`` il comando cerca un file di nome ``Makefile`` (N.B.: la stringa è case-sensitive) nella directory corrente, lo interpreta ed esegue le regole ivi contenute.

Alternativamente con ``make -f MakefileNAME``, imponiamo al comando ``make`` di interpretare un makefile di nome ``MakefileNAME``.

Una **regola** di un makefile segue la seguente sintassi:

```make
target: dipendenze
	[tab] comando di sistema
```

Dato un particolare **target**, ``make`` esegue i comandi presenti alla linea successiva, se le dipendenze sono tutte soddisfatte.
Le dipendenze sono semplicemente file da cui il target dipende. 

> **_N.B.:_** l'etichetta [tab] indica il carattere che si ottiene con il tasto TAB della tastiera (codice ASCII 9 in decimale e esadecimale). Tale carattere è necessario, altrimenti otterremo un errore di interpretazione.

Il file **Makefile** utilizzato in quest'esercitazione è il seguente:

```make
all: start

start: lib.o main.o
	gcc -o start lib.o main.o

lib.o: lib.c lib.h
	gcc -c lib.c

main.o: main.c lib.h
	gcc -c main.c

clean:
	rm -f *.o
	rm -f ./start
	rm -f *~
```

Possiamo notare che la prima regola (``all``) dipende dal file ``start``, la quale dipende a sua volta dai file ``lib.o`` e ``main.o``, e così via fino ad arrivare ad una regola risolvibile.

Per questioni di utilità, spesso viene sempre aggiunta una regola di **clean** all'interno di un Makefile. Questa regola (senza dipendenze) permette di effettuare una pulizia dei file oggetto e dei file eseguibili generati durante la compilazione. E.g.:

```make
clean:
	rm -rf *.o
	rm -rf executable
```
> **_N.B.:_** Osservare come viene utilizzata la wildcard ``*.o`` per indicare tutti i file che terminano con l'estensione ``.o``

Inoltre, in un Makefile è possibile utilizzare variabili. Una variabile è inizializzata usando la sintassi ``NOME_VARIABILE=VALORE``. Per accedere al valore della variabile basta utilizzare la sintassi ``$(NOME_VARIABILE)`` oppure ``${NOME_VARIABILE}``. Le vediamo all'opera nel file [``Makefile2``](Makefile2) (passo 6).


## Esperimenti da provare

### 1. Compilare ed eseguire

```console
$ make
gcc -c lib.c
gcc -c main.c
gcc -o start lib.o main.o
```

``make`` parte dalla prima regola (``all``), scende lungo le dipendenze e esegue prima i comandi delle "foglie" (``lib.o`` e ``main.o``), poi quello di ``start``.

```console
$ ./start
```

Input di prova: ``3`` elementi, poi ``1 2 3`` per il primo vettore e ``10 20 30`` per il secondo. Il programma stampa la somma: ``11 22 33``.

### 2. Che cosa viene ricompilato?

```console
$ make
make: Nothing to be done for 'all'.
$ touch lib.c
$ make
gcc -c lib.c
gcc -o start lib.o main.o
```

La seconda volta non c'è niente da fare: è tutto aggiornato. ``touch`` aggiorna solo la data di ultima modifica di ``lib.c``, come se l'avessimo modificato: ``make`` ricompila solo ``lib.o`` e ricollega ``start``. ``main.o`` resta com'è.

> **_N.B.:_** ``make`` decide cosa ricostruire confrontando le **date di ultima modifica** (timestamp) del target e delle sue dipendenze: se una dipendenza è più recente del target, il target va rigenerato.

> **_Domanda:_** e se modifichiamo ``lib.h``? Quali comandi verranno eseguiti? Provate a prevederlo guardando il Makefile, poi verificate.

```console
$ touch lib.h
$ make --trace
Makefile:7: update target 'lib.o' due to: lib.h
gcc -c lib.c
Makefile:10: update target 'main.o' due to: lib.h
gcc -c main.c
Makefile:4: update target 'start' due to: lib.o main.o
gcc -o start lib.o main.o
```

Si ricompilano **entrambi** i file oggetto, perché tutti e due dipendono da ``lib.h``. Con ``--trace``, ``make`` spiega il **perché** di ogni comando che esegue.

### 3. Dry-run e regole singole

```console
$ make -n clean
rm -f *.o
rm -f ./start
rm -f *~
```

Con ``-n`` (*dry-run*) ``make`` stampa i comandi che eseguirebbe, **senza eseguirli**: utilissimo prima di lanciare un ``clean`` scritto da qualcun altro.

```console
$ make clean
$ make lib.o
gcc -c lib.c
```

Si può chiedere a ``make`` un target preciso: ``make lib.o`` esegue solo la regola ``lib.o`` (e quelle da cui dipende), senza creare l'eseguibile.

### 4. Il tranello di ``clean`` (e perché serve ``.PHONY``)

```console
$ touch clean          # creo un file che si chiama proprio "clean"
$ make clean
make: 'clean' is up to date.
$ rm clean
```

Il target ``clean`` non ha dipendenze ed esiste un file con quel nome: per ``make`` è quindi "già aggiornato" e non esegue nulla. La soluzione è dichiarare i target che non corrispondono a file come **phony** (si veda [``Makefile2``](Makefile2)):

```make
.PHONY: all clean
```

### 5. L'errore più famoso: il TAB

Aprite il ``Makefile`` con un editor, sostituite il TAB davanti a un comando con degli spazi, salvate e lanciate ``make``:

```console
$ make
Makefile:4: *** missing separator (did you mean TAB instead of 8 spaces?).  Stop.
```

Poi ripristinate il file originale, annullando la modifica nell'editor oppure con ``git checkout Makefile``.

### 6. Variabili e regole implicite: ``Makefile2``

```make
CC = gcc
CFLAGS = -Wall -g
DEPS = lib.h
OBJ = lib.o main.o

.PHONY: all clean

all: start

start: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
```

- ``CC`` e ``CFLAGS`` sono i nomi convenzionali per il compilatore e le sue opzioni: per cambiarli basta modificare una riga.
- ``%.o: %.c $(DEPS)`` è una **regola implicita** (*pattern rule*): "ogni file ``.o`` si ottiene dal ``.c`` con lo stesso nome". Una sola regola sostituisce quelle di ``lib.o`` e ``main.o``.
- Le **variabili automatiche**: ``$@`` è il target, ``$^`` sono tutte le dipendenze, ``$<`` è la prima dipendenza. Per ``lib.o``, ``$@`` vale ``lib.o`` e ``$<`` vale ``lib.c``.

```console
$ make clean
$ make -f Makefile2
gcc -Wall -g -c -o lib.o lib.c
gcc -Wall -g -c -o main.o main.c
gcc -o start lib.o main.o
```

Le variabili si possono anche **sovrascrivere da linea di comando**, senza modificare il file:

```console
$ make -f Makefile2 clean
$ make -n -f Makefile2 CFLAGS=-O2
gcc -O2 -c -o lib.o lib.c
gcc -O2 -c -o main.o main.c
gcc -o start lib.o main.o
```
