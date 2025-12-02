# Server multi-processo con memoria condivisa

Si realizzi in linguaggio C/C++ una applicazione di tipo Client-Server
basata su **code di messaggi UNIX**. L'applicazione dovrà essere
separata in 2 eseguibili, rispettivamente per le parti Client e Server.

Il processo padre del Server (ossia il processo creato dal SO all'avvio
del primo eseguibile) dovrà creare un gruppo di processi figli (3 in
totale), i quali dovranno porsi in attesa di ricevere messaggi da parte
dei processi Client. Ogni messaggio Client-Server dovrà contenere un
valore intero casuale tra 1 e 10. I processi del Server (figli e padre)
dovranno condividere una **shared memory UNIX**, che conterrà due
variabili intere (entrambe da impostare inizialmente a zero). Ad ogni
ricezione di un messaggio, ogni processo figlio del Server dovrà
incrementare la prima variabile intera ( "totale") di una quantità pari
al valore contenuto nel messaggio; inoltre, dovrà incrementare la
seconda variabile ( "numero messaggi") di 1. Gli accessi concorrenti
alla shared memory dovranno essere disciplinati in **mutua esclusione**.
I processi figli del Server dovranno terminare dopo aver ricevuto e
servito 3 messaggi ciascuno. Il processo padre del Server dovrà
attendere la terminazione dei figli, per poi stampare il contenuto della
shared memory e terminare a sua volta.

Il processo Client (ossia il processo creato dal SO all'avvio del
secondo eseguibile, da lanciare in un terminale separato dal primo)
dovrà inviare una sequenza di 9 messaggi verso il Server, per poi
terminare. Si utilizzi per la comunicazione una send di tipo asincrono.

![image](/images/ambiente_locale/code_messaggi/server_multi-processo_con_memoria_condivisa.png)
