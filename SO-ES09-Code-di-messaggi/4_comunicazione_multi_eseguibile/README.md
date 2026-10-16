## Esercizio. Coda di messaggi con applicativi multipli

*Scrivere un'applicazione concorrente che sincronizzi tramite coda di messaggi POSIX tre tipologie di processi P1, P2, P3. I processi P1 e P2 inviano sulla coda un messaggio del tipo ``(PROCESS_ID, FLOAT_VALUE)``, dove il ``PROCESS_ID`` identifica il processo mittente (``P1`` o ``P2``), mentre FLOAT_VALUE è un valore float generato casualmente nell'intervallo ``[i_dx,i_sx]``.
Il processo P3 dovrà ricevere 22 messaggi, 11 da parte del processo P1 e 11 dal processo P2, e calcolare la media cumulativa delle 2 serie. Al termine dei 22 messaggi ricevuti, il processo P3 stampa a video le medie calcolate.*

Si noti che i tre eseguibili condividono la coda semplicemente aprendo lo stesso **nome** (`/so_es08_calc`, definito in [header.h](header.h)): non servono più chiavi generate da `ftok`. L'identificativo del mittente, che con altre famiglie di code era il campo "tipo" obbligatorio, qui è un normale campo della struttura messaggio.

Compilare ed eseguire il codice:

```console
$ make
$ ./start
Processo P1 avviato
Processo P2 avviato
Processo P3 avviato
Invio messaggio: <1,5.868036>
Ricevuto messaggio dal processo <1> ,con valore <5.868036>
...
<Media 1 = 5.868036>
<Media 2 = 11.736073>
```
