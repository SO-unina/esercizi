# Libreria statica

In questo esempio si vuole generare una libreria statica che racchiuda le funzionalità di somma e prodotto definite nei file ``mat.c`` e ``mat.h``.
Una libreria statica è una collezione di moduli oggetto (file ``.o``) in un unico file, con estensione ``.a``
Per creare una libreria statica si può usare il comando ``ar`` (archiver) di Unix.

La libreria analizzata in quest'esempio viene utilizzata nel codice ``main.c``.

Supponendo di chiamare la libreria statica ``mat``, per poter compilare tale libreria come statica è necessario eseguire il comando:

```
ar rcs libmat.a mat.o
```

In particolare, i flag specificano che:

- ``r``: inserisce il file con rimpiazzo
- ``c``: crea la libreria, se non esiste
- ``s``: crea un indice oggetto-file

Infine, per collegare i vari file oggetto con la libreria statica generata, bisogna utilizzare il compilatore ``gcc`` e i flag ``-L``, per indicare il path dove ricercare la libreria, e il flag ``-l`` per specificare il nome (epurato dal prefisso ``lib`` e dall'estensione ``.a``) della libreria da collegare. Ad esempio:

```
gcc -o calc main.c -L$(CURR) -lmat
```

Notare come nel Makefile viene utilizzata la variabile d'ambiente ``${PWD}`` per poter accedere al percorso corrente (dove si sta eseguendo il comando ``make``).

## Esperimenti da provare

### 1. Creare la libreria e collegarla

```console
$ make
gcc -c mat.c
ar rcs libmat.a mat.o
gcc -o calc main.c -L/percorso/della/cartella -lmat
$ ./calc
( 1 1 4 4 4 1 4 2 1 3  )
Somma = 25
Prod = 1536
```

### 2. Cosa c'è dentro la libreria

```console
$ ar t libmat.a
mat.o
$ nm libmat.a

mat.o:
0000000000000049 T prod
0000000000000000 T somma
```

``ar t`` elenca i file oggetto contenuti nell'archivio: qui c'è solo ``mat.o``, con le funzioni ``somma`` e ``prod``.

### 3. Il codice della libreria è stato copiato nell'eseguibile

```console
$ nm calc | grep -E "somma|prod"
0000000000001434 T prod
00000000000013eb T somma
$ rm libmat.a
$ ./calc
( 1 1 4 4 4 1 4 2 1 3  )
Somma = 25
Prod = 1536
$ make
```

In ``calc`` i simboli ``somma`` e ``prod`` sono ``T``, cioè definiti **dentro** l'eseguibile: il linker ha copiato il codice di ``mat.o`` dall'archivio. Per questo ``calc`` funziona anche dopo aver cancellato ``libmat.a``: la libreria statica serve solo al momento del collegamento. (L'ultimo ``make`` ricrea la libreria.)

### 4. Trappola classica: l'ordine degli argomenti

```console
$ gcc -o calc2 -L. -lmat main.c
/usr/bin/ld: /tmp/ccufqH4L.o: in function `main':
main.c:(.text+0x142): undefined reference to `somma'
main.c:(.text+0x156): undefined reference to `prod'
collect2: error: ld returned 1 exit status
$ gcc -o calc2 main.c -L. -lmat
```

Il linker legge gli argomenti **da sinistra a destra**. Quando incontra ``libmat.a`` nessuno ha ancora chiesto ``somma`` e ``prod``, quindi non estrae nulla dall'archivio; quando poi arriva ``main.c``, è troppo tardi. Le librerie vanno sempre **dopo** i file che le usano.
