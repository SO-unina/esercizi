# Libreria statica vs dinamica: lo stesso programma, tre eseguibili

In questo esempio lo stesso ``main.c`` viene collegato alla stessa libreria ``libmat`` in tre modi diversi:

| Eseguibile            | ``libmat``          | libreria C (``libc``) |
|-----------------------|---------------------|-----------------------|
| ``calc_statico``      | statica (``.a``)    | dinamica              |
| ``calc_dinamico``     | dinamica (``.so``)  | dinamica              |
| ``calc_tutto_statico``| statica (``.a``)    | statica (``-static``) |

La versione 1 della libreria ([``mat_v1.c``](mat_v1.c)) contiene un **bug**: ``somma`` salta il primo elemento del vettore.

Confrontiamo i tre eseguibili per **dimensione** (passo 1), **manutenzione** (passo 2) e **tempo di esecuzione** (passi 3 e 4).

## 1. Compilare e confrontare

```console
$ make
$ make confronto
=== Dimensioni (byte) ===
     16080  calc_dinamico
     16144  calc_statico
    785456  calc_tutto_statico

=== Dove si trova 'somma'? (T = dentro l'eseguibile, U = da cercare a runtime) ===
calc_statico        0000000000001244 T somma
calc_dinamico                        U somma
calc_tutto_statico  0000000000401940 T somma
...
```

Osservazioni:

- ``calc_tutto_statico`` è circa **50 volte più grande**: contiene al suo interno anche tutta la parte di ``libc`` che usa (``printf`` e tutto ciò da cui dipende).
- In ``calc_statico`` il codice di ``somma`` è stato **copiato dentro** l'eseguibile (``T``); in ``calc_dinamico`` è **undefined** (``U``): verrà cercato in ``libmat.so`` a tempo di caricamento.
- ``ldd calc_tutto_statico`` risponde ``not a dynamic executable``.

> **_N.B.:_** Se nella stessa cartella sono presenti sia ``libmat.a`` sia ``libmat.so``, l'opzione ``-lmat`` sceglie la libreria **dinamica**. Per forzare quella statica si usa ``-l:libmat.a`` (si veda il [``Makefile``](Makefile)).

## 2. Correggere il bug... senza ricompilare

```console
$ make esegui
[./calc_statico] libmat v1: somma(1..10) = 54 (atteso 55)
[./calc_dinamico] libmat v1: somma(1..10) = 54 (atteso 55)
[./calc_tutto_statico] libmat v1: somma(1..10) = 54 (atteso 55)

$ make aggiorna          # ricostruisce SOLO libmat.so a partire da mat_v2.c
$ make esegui
[./calc_statico] libmat v1: somma(1..10) = 54 (atteso 55)
[./calc_dinamico] libmat v2: somma(1..10) = 55 (atteso 55)
[./calc_tutto_statico] libmat v1: somma(1..10) = 54 (atteso 55)
```

Aggiornando la sola libreria dinamica, il programma che la usa si "aggiusta" **senza essere ricompilato né ricollegato**. I programmi collegati staticamente, invece, si portano dietro la vecchia copia del codice e vanno ricollegati.

È esattamente ciò che succede quando il sistema operativo installa un aggiornamento di sicurezza di una libreria come ``libssl``: tutti i programmi che la usano dinamicamente vengono corretti in un colpo solo (basta riavviarli).

## 3. Il tempo di avvio

```console
$ make avvio
1000 avvii di calc_statico:
  0,674 secondi
1000 avvii di calc_dinamico:
  0,720 secondi
1000 avvii di calc_tutto_statico:
  0,495 secondi
```

Ogni volta che parte un programma collegato dinamicamente, prima del programma lavora il loader dinamico: carica le librerie e collega i simboli. Un programma tutto statico non ne ha bisogno e parte subito. Su 1000 avvii la differenza si vede: circa 0,5 ms per avvio contro 0,7 ms (tempo che comprende anche la creazione del processo).

``calc_statico`` sta in mezzo: ``libmat`` è statica, ma la libc è ancora dinamica, e basta una sola libreria dinamica perché il loader debba entrare in gioco.

## 4. Il costo di ogni chiamata

Il programma [``ciclo.c``](ciclo.c) chiama un miliardo di volte una funzione volutamente banale della libreria ``libconta`` ([``conta.c``](conta.c)):

```c
for(i = 0; i < n; i++)
	tot = incrementa(tot);
```

Lo colleghiamo alla libreria statica (``ciclo_statico``) e a quella dinamica (``ciclo_dinamico``):

```console
$ make chiamate
ciclo_statico:
1000000000 chiamate a incrementa()
  1,059 secondi
ciclo_dinamico:
1000000000 chiamate a incrementa()
  1,893 secondi
```

Il motivo si vede nel codice macchina:

```console
$ objdump -d ciclo_statico | grep "call.*incrementa"
    10a0:	e8 3b 01 00 00       	call   11e0 <incrementa>
$ objdump -d ciclo_dinamico | grep "call.*incrementa"
    10c0:	e8 ab ff ff ff       	call   1070 <incrementa@plt>
```

Con la libreria statica la ``call`` salta direttamente alla funzione, che è stata copiata dentro l'eseguibile. Con quella dinamica passa dal "trampolino" ``incrementa@plt``, che salta all'indirizzo scritto dal loader in una tabella (lo stesso meccanismo visto per ``printf`` in [``2_elfdumping``](../2_elfdumping)). È un salto in più a ogni chiamata: meno di un nanosecondo, che su un miliardo di chiamate diventa quasi un secondo.

> **_N.B.:_** I tempi cambiano da macchina a macchina (e su un portatile possono variare da un'esecuzione all'altra): conviene lanciare ``make avvio`` e ``make chiamate`` un paio di volte. Le proporzioni, però, restano le stesse.

## In sintesi

| | Statica | Dinamica |
|---|---|---|
| Dimensione dell'eseguibile | più grande: contiene il codice della libreria | più piccolo |
| Aggiornare la libreria | bisogna ricollegare ogni programma | basta sostituire il ``.so`` |
| Avvio | più veloce: niente loader | più lento: il loader carica e collega le librerie |
| Ogni chiamata a una funzione di libreria | chiamata diretta | un salto in più (``@plt``) |

Le librerie dinamiche costano qualcosa in tempo. Restano comunque la scelta di default perché nella maggior parte dei programmi questi costi sono trascurabili, mentre i vantaggi (aggiornamenti, spazio su disco e in memoria) sono grandi.
