# Dal sorgente all'eseguibile: le fasi di ``gcc``

Quando scriviamo ``gcc saluti.c -o saluti``, ``gcc`` esegue in realtà **quattro programmi diversi** uno dopo l'altro:

```
saluti.c --[preprocessore]--> saluti.i --[compilatore]--> saluti.s --[assemblatore]--> saluti.o --[linker]--> saluti
  (C)          gcc -E           (C espanso)   gcc -S        (assembly)    gcc -c        (oggetto ELF)  gcc      (eseguibile ELF)
```

Il [``Makefile``](Makefile) di questa cartella ha **una regola per ogni fase**, così da poter osservare ogni file intermedio:

```console
$ make
gcc -E saluti.c -o saluti.i
gcc -S saluti.i -o saluti.s
gcc -c saluti.s -o saluti.o
gcc saluti.o -o saluti
$ ./saluti
Ciao! Benvenuti al corso di Sistemi Operativi 2026
QUADRATO(1+2)           = 9
QUADRATO_SBAGLIATO(1+2) = 5
```

> **_N.B.:_** La sintassi dei Makefile la vediamo nell'esempio [``1_makefile_esempio``](../1_makefile_esempio): per ora basta sapere che ogni regola corrisponde a una fase.

## 1. Preprocessore (``gcc -E``)

Il preprocessore lavora **solo sul testo**: include i file ``#include``, sostituisce le macro ``#define``, valuta ``#ifdef``/``#if``.

```console
$ wc -l saluti.c saluti.i
   40 saluti.c
  831 saluti.i          # l'#include <stdio.h> ha "incollato" centinaia di righe!
$ tail -9 saluti.i      # le macro sono sparite, sostituite dal loro valore
...
 printf("%s! Benvenuti al corso di Sistemi Operativi %d\n", "Ciao", 2026);
 printf("QUADRATO(1+2)           = %d\n", ((1+2) * (1+2)));
 printf("QUADRATO_SBAGLIATO(1+2) = %d\n", 1+2 * 1+2);
...
```

``ANNO`` è diventato ``2026`` e ``SALUTO`` è diventato ``"Ciao"``. Le macro vengono sostituite **alla lettera**: ``QUADRATO_SBAGLIATO(1+2)``, che non ha parentesi, diventa ``1+2 * 1+2``. Il preprocessore non conosce il C: fa solo sostituzioni di testo.

**Compilazione condizionale**: lo stesso sorgente produce programmi diversi a seconda delle macro definite. Con ``-D`` possiamo definirle da linea di comando:

```console
$ make macchina                                 # gcc -DMACCHINA ...
Ciao, macchina! Sono un programma per CPU x86-64, compilato con gcc 13.3 il Sep 23 2026
$ gcc -DSALUTO='"Hello"' saluti.c -o saluti && ./saluti
```

Altre macro le definisce ``gcc`` da solo, in base alla macchina per cui sta compilando: ``__x86_64__`` (``__aarch64__`` su ARM), ``__linux__``, ``__GNUC__``, ``__DATE__``, ... Il saluto della modalità ``MACCHINA`` è costruito proprio con queste. Per vederle tutte:

```console
$ gcc -dM -E - < /dev/null | less
```

## 2. Compilatore (``gcc -S``)

```console
$ less saluti.s        # codice assembly: cercate "call printf"
```

## 3. Assemblatore (``gcc -c``)

```console
$ file saluti.o
saluti.o: ELF 64-bit LSB relocatable, x86-64 ...
$ nm saluti.o
0000000000000000 T main
                 U printf      # U = Undefined: printf non e' qui dentro, la trovera' il linker
```

Notate: il file oggetto è **rilocabile** (``relocatable``) e ``main`` si trova all'indirizzo ``0``: gli indirizzi definitivi verranno assegnati dal linker.
Il comando nm in Linux viene utilizzato per elencare i simboli (funzioni, variabili globali, costanti, ecc.) contenuti all'interno di file oggetto, librerie statiche (.a) e file binari eseguibili.
## 4. Linker

```console
$ file saluti
saluti: ELF 64-bit LSB pie executable, x86-64, ..., dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2 ...
```

L'eseguibile è **collegato dinamicamente** alla libreria C e verrà caricato da ``/lib64/ld-linux-x86-64.so.2``: ne parleremo nella parte sulle librerie.

> **_Suggerimento:_** ``gcc -save-temps saluti.c -o saluti`` genera tutti i file intermedi (``.i``, ``.s``, ``.o``) in un colpo solo.
