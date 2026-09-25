# Dentro un file ELF: file oggetto, simboli e lavoro del linker

In questo esempio usiamo gli strumenti di *binutils* per guardare dentro i file ELF:

| Comando | Cosa mostra |
|---------|-------------|
| ``objdump -x`` | tutti gli header: sezioni e tabella dei simboli |
| ``objdump -d`` | il codice macchina disassemblato (con ``-S`` anche il sorgente C, con ``-r`` anche le rilocazioni) |
| ``nm`` | la tabella dei simboli, in forma compatta |
| ``readelf -h``, ``-l``, ``-r`` | header ELF, segmenti, rilocazioni dinamiche |

Nella **parte 1** esploriamo un singolo file oggetto (``mat.o``). Nella **parte 2** vediamo cosa fa il linker quando unisce ``main.o`` e ``mat.o`` nell'eseguibile ``calc``.

> **_N.B.:_** Gli output riportati sono stati ottenuti con gcc 13 e binutils su x86-64. Sul vostro sistema indirizzi e dimensioni possono essere diversi, ma i ragionamenti (e i conti) restano gli stessi. Con la lingua del sistema in italiano, alcune etichette di ``objdump`` sono tradotte (es. ``Sezioni`` invece di ``Sections``).

## Parte 1: dentro un file oggetto

### 1. Compilare senza collegare

```console
$ gcc -c -g mat.c
```

- ``-c`` ferma ``gcc`` dopo l'assemblatore: produce il file oggetto ``mat.o`` senza provare a creare un eseguibile (che fallirebbe: in ``mat.c`` non c'è ``main``).
- ``-g`` aggiunge le informazioni di debug, che servono a ``objdump -S`` per mostrare il codice C accanto all'assembly (e a ``gdb``).

### 2. Header e sezioni: ``objdump -x``

Ogni file ELF (file oggetto, eseguibile, libreria, core dump) inizia con gli stessi quattro byte, il *magic number*:

```console
$ head -c 4 mat.o | xxd
00000000: 7f45 4c46                                .ELF
```

(Se ``xxd`` non è installato: ``head -c 4 mat.o | od -c``.) Il resto dell'header e le sezioni le mostra ``objdump -x``:

```console
$ objdump -x mat.o

mat.o:     file format elf64-x86-64
mat.o
architecture: i386:x86-64, flags 0x00000011:
HAS_RELOC, HAS_SYMS
start address 0x0000000000000000

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000098  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         00000000  0000000000000000  0000000000000000  000000d8  2**0
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  000000d8  2**0
                  ALLOC
  3 .debug_info   00000124  0000000000000000  0000000000000000  000000d8  2**0
...
SYMBOL TABLE:
0000000000000000 l    df *ABS*	0000000000000000 mat.c
0000000000000000 l    d  .text	0000000000000000 .text
...
0000000000000000 g     F .text	0000000000000049 somma
0000000000000049 g     F .text	000000000000004f prod
```

**L'intestazione**:

- ``elf64-x86-64``: file ELF a 64 bit per processori x86-64;
- ``HAS_RELOC, HAS_SYMS``: il file contiene **rilocazioni** e una **tabella dei simboli**, cioè informazioni destinate al linker (le vediamo nella parte 2);
- ``start address 0x0``: un file oggetto non ha un punto di ingresso, non è eseguibile.

**Le sezioni** (``Size`` e gli indirizzi sono in esadecimale):

- ``.text`` è lunga ``0x98`` = 152 byte: è il codice macchina di ``somma`` e ``prod``;
- ``.data`` (variabili globali inizializzate) e ``.bss`` (variabili globali non inizializzate) sono vuote, perché ``mat.c`` non ha variabili globali. Notate che ``.bss`` ha il flag ``ALLOC`` ma non ``CONTENTS``: occupa spazio in memoria, ma non nel file (tanto sono tutti zeri);
- le sezioni ``.debug_*`` sono state aggiunte da ``-g``;
- la colonna ``VMA`` (l'indirizzo in memoria) vale ``0`` per tutte le sezioni: non sono ancora state collocate da nessuna parte. È il linker che deciderà dove metterle.

**La tabella dei simboli**. Le colonne sono: valore (per una funzione, la posizione all'interno della sua sezione), flag (``g`` globale, ``l`` locale, ``F`` funzione, ``O`` oggetto/variabile, ``d`` debug, ``f`` file), sezione, dimensione, nome. ``somma`` inizia a ``0`` e occupa ``0x49`` byte; ``prod`` inizia subito dopo, a ``0x49``, e occupa ``0x4f`` byte: ``0x49 + 0x4f = 0x98``, cioè tutta la ``.text``.

> **_Prova:_** aggiungete in fondo a ``mat.c`` le righe ``int contatore = 5;`` e ``int buffer[100];``, ricompilate e rilanciate ``objdump -x``. La ``.data`` diventa di 4 byte (un ``int``), la ``.bss`` di ``0x190`` = 400 byte (100 ``int``), e nella tabella dei simboli compaiono ``contatore`` e ``buffer`` con il flag ``O``.

### 3. Il codice macchina: ``objdump -d -S``

```console
$ objdump -d -S mat.o

Disassembly of section .text:

0000000000000000 <somma>:
#include <stdio.h>
#include <stdlib.h>
#include "mat.h"

int somma(int* s,int d){
   0:	f3 0f 1e fa          	endbr64
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
   c:	89 75 e4             	mov    %esi,-0x1c(%rbp)
	int temp = 0;
   f:	c7 45 f8 00 00 00 00 	movl   $0x0,-0x8(%rbp)
	int i;
	for(i=0; i < d; i++){
  16:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
  1d:	eb 1d                	jmp    3c <somma+0x3c>
...
```

Per ogni istruzione ci sono tre colonne: la posizione dall'inizio della ``.text`` (parte da ``0``: il codice è ancora **rilocabile**), i byte dell'istruzione e l'assembly corrispondente. Grazie a ``-S``, sopra ogni gruppo di istruzioni compare la riga C che le ha generate: ad esempio ``int temp = 0;`` diventa ``movl $0x0,-0x8(%rbp)``, cioè "scrivi 0 nella variabile locale che si trova sullo stack, 8 byte sotto ``%rbp``".

## Parte 2: il linker all'opera

``main.c`` chiama ``somma`` e ``prod``, che sono definite in ``mat.c``. Compilando i due file separatamente, quando ``gcc`` traduce ``main.c`` **non sa** dove si troverà ``somma`` nell'eseguibile finale. Che cosa scrive, allora, al posto dell'indirizzo?

### 4. Simboli definiti e simboli mancanti: ``nm``

```console
$ gcc -c main.c mat.c
$ nm main.o
                 U free
00000000000000ea T main
                 U malloc
0000000000000000 T popola
                 U printf
                 U prod
                 U puts
                 U rand
                 U somma
                 U srand
                 U __stack_chk_fail
000000000000006f T stampa
                 U time
$ nm mat.o
0000000000000049 T prod
0000000000000000 T somma
```

- ``T``: simbolo **definito** nella sezione ``.text`` di questo file (con la sua posizione);
- ``U`` (*undefined*): simbolo **usato ma non definito** qui. ``main.o`` ha bisogno di ``somma`` e ``prod`` (che stanno in ``mat.o``) e di ``printf``, ``malloc``, ``rand``, ... (che stanno nella libreria C).

Il primo compito del linker è far combaciare ogni ``U`` con una ``T`` in un altro file. Se non la trova, si ottiene l'errore ``undefined reference to ...``; se ne trova due, ``multiple definition of ...``.

> **_N.B.:_** Altre lettere che potete incontrare: ``D`` (variabile in ``.data``), ``B`` (variabile in ``.bss``), ``R`` (costante in ``.rodata``). Le minuscole indicano simboli locali, ad esempio le funzioni ``static``.

### 5. Il segnaposto e la rilocazione: ``objdump -dr``

Con ``-r``, sotto ogni istruzione ``objdump`` mostra le eventuali rilocazioni. Cerchiamo la chiamata a ``somma``:

```console
$ objdump -dr main.o | grep -B1 -A1 "somma-"
 141:	e8 00 00 00 00       	call   146 <main+0x5c>
			142: R_X86_64_PLT32	somma-0x4
 146:	89 45 d0             	mov    %eax,-0x30(%rbp)
```

**L'istruzione.** Su x86-64 una ``call`` occupa 5 byte: ``e8`` è il codice dell'operazione, i 4 byte successivi sono lo **spostamento**, cioè di quanto saltare contando dall'inizio dell'istruzione successiva (qui ``0x146``). In ``main.o`` lo spostamento vale ``00 00 00 00``: è un **segnaposto**, perché il compilatore non sa dove sarà ``somma``. (Il ``call 146 <main+0x5c>`` stampato da ``objdump`` non ha significato: uno spostamento 0 "salta" all'istruzione successiva.)

**La rilocazione** (la riga sotto) è una nota lasciata al linker:

| Campo | Significato |
|-------|-------------|
| ``142`` | **dove** scrivere: i 4 byte subito dopo ``e8`` (``0x141 + 1``) |
| ``R_X86_64_PLT32`` | **cosa** scrivere: uno spostamento a 32 bit, relativo alla posizione corrente |
| ``somma-0x4`` | **verso quale simbolo**, con una correzione di -4: lo spostamento si conta dalla fine dell'istruzione, che è 4 byte dopo l'inizio del campo |

Con ``objdump -dr main.o | grep -A1 call`` si vede che **ogni** ``call`` ha il suo segnaposto e la sua rilocazione: verso ``malloc``, ``printf``, ``time``, ... e anche verso ``popola`` e ``stampa``, che pure stanno nello stesso file (le chiamate a funzioni globali vengono comunque lasciate al linker). Non solo le chiamate: anche gli indirizzi delle stringhe passate a ``printf`` sono segnaposti (``lea 0x0(%rip),%rax`` con rilocazione ``R_X86_64_PC32 .rodata+...``). L'elenco completo è in ``objdump -r main.o``.

### 6. Il collegamento

```console
$ gcc -o calc main.o mat.o
$ nm -n calc | grep " T "
0000000000001000 T _init
0000000000001140 T _start
0000000000001229 T popola
0000000000001298 T stampa
0000000000001313 T main
00000000000013eb T somma
0000000000001434 T prod
0000000000001484 T _fini
```

Il linker ha fatto due lavori.

**a) Mettere in fila le sezioni e assegnare gli indirizzi.** Ha concatenato la ``.text`` di ``main.o`` (lunga ``0x1c2`` byte, lo dice ``objdump -h main.o``), quella di ``mat.o`` e il codice di avvio della libreria C (``_start``, ``_init``, ...). A questo punto ogni simbolo ha un indirizzo definitivo:

| Simbolo | Posizione nel file oggetto | Indirizzo in ``calc`` |
|---------|----------------------------|-----------------------|
| ``popola`` | ``main.o`` + ``0x0`` | ``0x1229`` (inizio della ``.text`` di ``main.o``) |
| ``stampa`` | ``main.o`` + ``0x6f`` | ``0x1298`` = ``0x1229 + 0x6f`` |
| ``main`` | ``main.o`` + ``0xea`` | ``0x1313`` = ``0x1229 + 0xea`` |
| ``somma`` | ``mat.o`` + ``0x0`` | ``0x13eb`` = ``0x1229 + 0x1c2`` (subito dopo ``main.o``) |
| ``prod`` | ``mat.o`` + ``0x49`` | ``0x1434`` = ``0x13eb + 0x49`` |

È esattamente la figura della slide *Linking*: i moduli oggetto vengono messi uno dopo l'altro nel *load module*.

**b) Applicare le rilocazioni.** Ora che ``somma`` ha un indirizzo, il linker può riempire il segnaposto:

```console
$ objdump -d calc | grep -B1 -A1 "call.*<somma>"
    1367:	48 89 c7             	mov    %rax,%rdi
    136a:	e8 7c 00 00 00       	call   13eb <somma>
    136f:	89 45 d0             	mov    %eax,-0x30(%rbp)
```

Il conto che fa il linker per una rilocazione ``R_X86_64_PLT32`` è **S + A − P**:

- **P**, la posizione del campo da riempire: la ``call`` era a ``0x141`` in ``main.o``, quindi in ``calc`` è a ``0x1229 + 0x141 = 0x136a``, e il campo inizia un byte dopo, a ``0x136b``;
- **S**, l'indirizzo del simbolo: ``somma`` = ``0x13eb``;
- **A**, la correzione scritta nella rilocazione: ``-4``.

``S + A − P = 0x13eb − 4 − 0x136b = 0x7c``. Su x86-64 i numeri si scrivono in memoria dal byte meno significativo (*little-endian*), quindi i 4 byte diventano ``7c 00 00 00``: proprio quelli che vediamo.

**Verifica:** quando la CPU esegue la ``call``, l'istruzione successiva è a ``0x136f``, e ``0x136f + 0x7c = 0x13eb``, cioè l'inizio di ``somma``.

> **_Curiosità:_** la chiamata a ``popola`` in ``calc`` è ``e8 cb fe ff ff``. Lo spostamento ``0xfffffecb`` è un numero negativo (``-0x135``, in complemento a due): ``popola`` si trova **prima** di ``main``, quindi il salto è all'indietro.

> **_Domanda:_** in ``calc`` adesso c'è il codice di ``somma`` e di ``prod``. E quello di ``printf``? Provate a rispondere prima di proseguire.

### 7. E ``printf``? Il collegamento dinamico

```console
$ nm calc | grep printf
                 U printf@GLIBC_2.2.5
$ objdump -d calc | grep "call.*printf"
    12b7:	e8 34 fe ff ff       	call   10f0 <printf@plt>
...
$ objdump -d calc | grep -A2 "<printf@plt>:"
00000000000010f0 <printf@plt>:
    10f0:	f3 0f 1e fa          	endbr64
    10f4:	ff 25 b6 2e 00 00    	jmp    *0x2eb6(%rip)        # 3fb0 <printf@GLIBC_2.2.5>
$ readelf -r calc | grep printf
000000003fb0  000600000007 R_X86_64_JUMP_SLO 0000000000000000 printf@GLIBC_2.2.5 + 0
```

``printf`` è ancora ``U``: il suo codice non è in ``calc``, ma nella libreria dinamica ``libc.so.6``. Il linker allora fa puntare le ``call`` a un piccolo "trampolino" dentro ``calc``, ``printf@plt``, che salta all'indirizzo scritto in una tabella all'indirizzo ``0x3fb0`` (la *GOT*, *Global Offset Table*). Quell'indirizzo però non si conosce ancora: il linker ha lasciato un'altra rilocazione (``R_X86_64_JUMP_SLOT``), che verrà risolta dal loader dinamico ``ld-linux.so`` **quando il programma viene avviato**. È il collegamento dinamico che vediamo negli esempi [``4_dynlib``](../4_dynlib) e [``4b_statica_vs_dinamica``](../4b_statica_vs_dinamica).

### 8. File oggetto ed eseguibile a confronto: ``readelf -h``

```console
$ readelf -h main.o | grep -E "Type|Entry"
  Type:                              REL (Relocatable file)
  Entry point address:               0x0
$ readelf -h calc | grep -E "Type|Entry"
  Type:                              DYN (Position-Independent Executable file)
  Entry point address:               0x1140
$ nm calc | grep -w _start
0000000000001140 T _start
```

- ``main.o`` è ``REL``, cioè rilocabile: non ha un punto di ingresso.
- ``calc`` è ``DYN``, cioè un eseguibile *position-independent* (PIE): a ogni esecuzione il loader lo carica a un indirizzo diverso (ASLR, per rendere più difficili gli attacchi), e gli indirizzi visti finora (``0x1229``, ``0x13eb``, ...) sono relativi all'inizio del programma. Lo si vede con ``/proc/self/maps``, che mostra la mappa della memoria del processo che lo legge: lanciando tre volte ``head``, il suo eseguibile finisce ogni volta a un indirizzo diverso.

  ```console
  $ for i in 1 2 3; do head -1 /proc/self/maps; done
  5c5cbd8c9000-5c5cbd8cb000 r--p 00000000 103:05 30292577      /usr/bin/head
  64a328dbc000-64a328dbe000 r--p 00000000 103:05 30292577      /usr/bin/head
  5e5a86131000-5e5a86133000 r--p 00000000 103:05 30292577      /usr/bin/head
  ```

- Il punto di ingresso (``0x1140``) **non è** ``main``, ma ``_start``: il codice di avvio della libreria C, che prepara ``argc`` e ``argv`` e poi chiama ``main``.

### 9. Sezioni e segmenti: ``readelf -l``

```console
$ readelf -lW calc
...
  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
  LOAD           0x000000 0x0000000000000000 0x0000000000000000 0x0007b0 0x0007b0 R   0x1000
  LOAD           0x001000 0x0000000000001000 0x0000000000001000 0x000491 0x000491 R E 0x1000
  LOAD           0x002000 0x0000000000002000 0x0000000000002000 0x000200 0x000200 R   0x1000
  LOAD           0x002d80 0x0000000000003d80 0x0000000000003d80 0x000290 0x000298 RW  0x1000
...
 Section to Segment mapping:
  Segment Sections...
   03     .init .plt .plt.got .plt.sec .text .fini
   05     .init_array .fini_array .dynamic .got .data .bss
...
```

Nell'eseguibile il linker ha raggruppato le sezioni in **segmenti** con gli stessi permessi: il codice (``.text``, ``.plt``, ...) in un segmento ``LOAD`` leggibile ed eseguibile (``R E``), i dati (``.data``, ``.bss``, ``.got``) in uno leggibile e scrivibile (``RW``). Il loader carica in memoria i segmenti, non le sezioni: le sezioni sono la "vista di linking" del file ELF, i segmenti la "vista di esecuzione".
