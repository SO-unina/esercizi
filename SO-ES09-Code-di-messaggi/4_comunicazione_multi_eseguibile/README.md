## Esercizio. Coda di messaggi con applicativi multipli

*Scrivere un'applicazione concorrente che sincronizzi tramite coda di messaggi POSIX tre tipologie di processi P1, P2, P3. I processi P1 e P2 inviano sulla coda un messaggio del tipo ``(PROCESS_ID, FLOAT_VALUE)``, dove il ``PROCESS_ID`` identifica il processo mittente (``P1`` o ``P2``), mentre FLOAT_VALUE è un valore float generato casualmente nell'intervallo ``[i_sx,i_dx]``.
Il processo P3 dovrà ricevere 22 messaggi, 11 da parte del processo P1 e 11 dal processo P2, e calcolare la media cumulativa delle 2 serie. Al termine dei 22 messaggi ricevuti, il processo P3 stampa a video le medie calcolate.*

Si noti che i tre eseguibili condividono la coda semplicemente aprendo lo stesso **nome** (`/so_es08_calc`, definito in [header.h](header.h)): non servono più chiavi generate da `ftok`. L'identificativo del mittente, che con altre famiglie di code era il campo "tipo" obbligatorio, qui è un normale campo della struttura messaggio.

Compilare ed eseguire il codice:

```console
$ make
$ ./start
Processo P2 avviato
Invio messaggio: <2,2.357294>
Processo P1 avviato
Invio messaggio: <1,1.606117>
Processo P3 avviato
ID QUEUE: 3
Ricevuto messaggio dal processo <2> ,con valore <2.357294>
Ricevuto messaggio dal processo <1> ,con valore <1.606117>
...
<Media 1 = 3.956218>
<Media 2 = 12.562388>
```

I valori sono casuali e cambiano a ogni esecuzione, come l'ordine in cui partono i processi.

> **_N.B.:_** P1 e P2 inizializzano il generatore con il proprio PID (``srand(getpid())``). Con ``srand(time(NULL))``, avviati nello stesso secondo, avrebbero lo stesso seme e quindi la stessa sequenza di numeri casuali.
