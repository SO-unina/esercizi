# Processo interfaccia, con thread produttore-consumatore

Si realizzi in linguaggio C/C++ una applicazione di tipo Client-Server
basata su **code di messaggi UNIX**. Oltre ad un processo Server e ad un
gruppo di processi Client, l'applicazione dovrà creare un ulteriore
processo, denominato **Interfaccia**, che faccia da mediatore tra i
Client e il Server. L'Interfaccia protegge il Server dal sovraccarico di
messaggi, bufferizzando e rigettando i messaggi in eccesso come
descritto di seguito.

Il processo Interfaccia dovrà creare due threads. Il primo thread dovrà
raccogliere i messaggi provenienti dai Client, e depositarli in un
**vettore di buffer circolare** (di 5 elementi). Nel caso in cui vi sia
spazio disponibile nel vettore, il thread dovrà depositare il messaggio
nel vettore, e subito dopo mandare una **risposta di conferma** al
Client (utilizzando il PID del Client nel campo-tipo del messaggio).
Invece, nel caso in cui non vi sia spazio disponibile, il thread **non**
dovrà porsi in attesa, ma dovrà ignorare il messaggio ricevuto e mandare
immediatamente una **risposta di rifiuto** al Client.

Il secondo thread del processo Interfaccia dovrà prelevare, uno alla
volta, i messaggi eventualmente presenti nel vettore di buffer, ed
inviarli al Server. Nel caso il vettore di buffer sia vuoto, il thread
dovrà sospendersi in attesa di messaggi nel vettore.

Il programma dovrà creare 3 processi Client, ognuno dei quali genererà 3
messaggi (senza attesa tra un messaggio e l'altro) e prelevando 3
risposte. Ogni messaggio di richiesta dovrà contenere un valore intero
scelto a caso. Il processo Server, quando riceve messaggi di richiesta,
dovrà stampare il PID e il valore del messaggio, e attendere un secondo
prima di ricevere il messaggio successivo. Si suggerisce di collocare il
codice dei processi in un unico eseguibile. Non è richiesto di
implementare una politica di uscita dei processi (è sufficiente che la
terminazione sia forzata manualmente dall'utente).

![image](/images/ambiente_locale/server_multithread/processo_interfaccia_con_produttore-consumatore.png)
