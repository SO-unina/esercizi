# Libreria dinamica (shared library)

In questo esempio si vuole generare una libreria dinamica che racchiuda le funzionalità di somma e prodotto definite nel file ``mat.c`` e ``mat.h``. Tale libreria viene utilizzata nel codice ``main.c``

Supponendo di chiamare la libreria mat, per poter compilare tale libreria come dinamica è necessario eseguire il comando:

```
gcc -shared -Wl,-soname,libmat.so -o libmat.so mat.o
```

Dove:
- ``-shared``: indica al compilatore che si vuole creare uno shared object
- ``-Wl``: passa opzioni al linker, in questo caso il nome della libreria
- ``-o``: nome da dare al file contenente la shared library

Per il nome della libreria vale una convenzione simile a quella vista per le librerie statiche: prefisso _lib_, e suffisso _.so_.
I file _obj1.o_, ..., _objN.o_ che compongono la libreria vanno compilati con l’opzione ``-fpic`` per generare codice rilocabile, ovvero indipendente dalla posizione (PIC – position independent code).

Notare come nel Makefile viene utilizzata la variabile d'ambiente ``${PWD}`` per poter accedere al percorso corrente (dove si sta eseguendo il comando ``make``).


## Esperimenti da provare

### 1. Compilare la libreria e il programma

```console
$ make -f MakefileShLib
gcc -c -fpic mat.c
gcc -shared -Wl,-soname,libmat.so -o libmat.so mat.o
gcc -c main.c
gcc -o calc main.o -L/percorso/della/cartella/ -lmat
```

(Con ``make -f MakefileShLib clean`` si rimuovono gli artefatti di compilazione.)

### 2. Il loader non trova la libreria

```console
$ ./calc
./calc: error while loading shared libraries: libmat.so: cannot open shared object file: No such file or directory
$ ldd calc
	linux-vdso.so.1 (0x000076a1d1d01000)
	libmat.so => not found
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x000076a1d1a00000)
	/lib64/ld-linux-x86-64.so.2 (0x000076a1d1d03000)
```

Il **linker** ha trovato ``libmat.so`` (grazie a ``-L``), ma a tempo di esecuzione è il **loader dinamico** a doverla cercare, e lui non sa nulla di ``-L``: cerca le librerie nelle cartelle di sistema (``/lib``, ``/usr/lib`` e quelle elencate in ``/etc/ld.so.conf``), nelle cartelle di ``LD_LIBRARY_PATH`` e nel percorso eventualmente scritto dentro l'eseguibile (*rpath*). ``ldd`` mostra le librerie dinamiche di cui ha bisogno il programma, e dove il loader le trova.

### 3. ``LD_LIBRARY_PATH``

```console
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD
$ ldd calc
	linux-vdso.so.1 (0x000073d98d726000)
	libmat.so (0x000073d98d714000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x000073d98d400000)
	/lib64/ld-linux-x86-64.so.2 (0x000073d98d728000)
$ ./calc
```

Ora il loader trova la libreria e il programma parte.

> **_N.B.:_** Se ``LD_LIBRARY_PATH`` era vuota, dopo l'``export`` vale ``:/percorso/della/cartella``: l'elemento vuoto prima dei ``:`` indica la **cartella corrente**, ed è lì che il loader trova ``libmat.so``. Per questo ``ldd`` stampa solo ``libmat.so`` e non il percorso completo. Lanciando ``ldd`` da un'altra cartella si vedrebbe ``libmat.so => /percorso/della/cartella/libmat.so``.

### 4. ``rpath``: il percorso scritto dentro l'eseguibile

Per evitare di dover modificare ``LD_LIBRARY_PATH`` si può usare il flag ``-Wl,-rpath,<cartella>``, che scrive il percorso della libreria dentro l'eseguibile. Osservate come è fatto il file [``MakefileShLibDyn``](MakefileShLibDyn), poi:

```console
$ unset LD_LIBRARY_PATH
$ make -f MakefileShLib clean
$ make -f MakefileShLibDyn
$ ./calc
$ readelf -d calc | grep -E "NEEDED|RUNPATH"
 0x0000000000000001 (NEEDED)             Shared library: [libmat.so]
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
 0x000000000000001d (RUNPATH)            Library runpath: [/percorso/della/cartella/]
```

> **_N.B.:_** L'``unset`` è importante: se ``LD_LIBRARY_PATH`` fosse ancora impostata, ``calc`` partirebbe comunque e non si vedrebbe l'effetto dell'rpath.

``calc`` parte senza ``LD_LIBRARY_PATH``. ``readelf -d`` mostra la sezione ``.dynamic`` dell'eseguibile, cioè le informazioni per il loader: le librerie di cui ha bisogno (``NEEDED``, con il nome scelto con ``-soname``) e il percorso in cui cercarle (``RUNPATH``).

### 5. Curiosità: quanti processi stanno usando la libc?

```console
$ grep -l libc.so.6 /proc/[0-9]*/maps 2>/dev/null | wc -l
163
```

``/proc/<PID>/maps`` elenca le zone di memoria di ogni processo: qui contiamo i processi (dell'utente corrente) che hanno la libc mappata in memoria. Sono centinaia, eppure il codice della libc in RAM c'è una volta sola: le sue pagine sono condivise tra tutti i processi.
