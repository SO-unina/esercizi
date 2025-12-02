# Server sincrono con processo di interfaccia asincrono

Si realizzi in linguaggio C/C++ una applicazione di tipo Client-Server
basata su **code di messaggi UNIX**. Oltre ad un processo Server e ad un
gruppo di processi Client, l'applicazione dovrà creare un ulteriore
processo, denominato **Interfaccia**, che faccia da mediatore tra i
Client e il Server.

Il processo Interfaccia dovrà raccogliere i messaggi provenienti dai
Client inviati tramite **send asincrona**. I messaggi ricevuti dovranno
essere a loro volta inviati al Server tramite send sincrona. Infine, il
processo Interfaccia dovrà mandare una **risposta di conferma** al
Client (utilizzando il PID del Client nel campo-tipo del messaggio).

Il programma dovrà creare 3 processi Client, ognuno dei quali genererà 3
messaggi (senza attesa tra un messaggio e l'altro) e prelevando 3
risposte. Ogni messaggio di richiesta dovrà contenere un valore intero
scelto a caso. Il processo Server, quando riceve messaggi di richiesta,
dovrà stampare il PID e il valore del messaggio, e attendere un secondo
prima di gestire il messaggio successivo. Il codice dei processi dovrà
essere collocato in 3 eseguibili distinti. I processi dovranno terminare
una volta che siano stati ricevuti o inviati tutti i messaggi previsti.

![image](/images/ambiente_locale/code_messaggi/server_sincrono_con_processo_di_interfaccia_asincrono.png)
