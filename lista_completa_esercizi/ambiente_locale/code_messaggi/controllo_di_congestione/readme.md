# Controllo di congestione

Si realizzi in C/C++ una applicazione **multiprocesso** basata su **code
di messaggi UNIX**, che realizzi uno schema **produttore-consumatore con
buffer multipli**. In questa applicazione, il vettore di buffer sarà
rappresentato da una coda di messaggi, su cui i processi produttori
dovranno accumulare al più 10 messaggi. Se la coda contiene già 10
messaggi, i produttori dovranno sospendersi in attesa che almeno un
messaggio sia consumato. I processi consumatori dovranno prelevare
messaggi da questa coda. Se la coda non contiene alcun messaggio, i
consumatori dovranno sospendersi in attesa che sia prodotto almeno un
messaggio.

Per sincronizzare i processi, si adotti il seguente protocollo. Il
processo padre, **prima** di avviare i processi produttori e
consumatori, dovrà creare una ulteriore **coda dei token** e inserirvi
10 messaggi token ("gettone"). I processi produttori, prima di produrre
un messaggio, dovranno prelevare un token dalla apposita coda; se non vi
sono token, i produttori dovranno sospendersi in attesa di ricevere
token. I processi consumatori, dopo aver consumato un messaggio, devono
inviare un token sulla apposita coda prima di consumare il messaggio
successivo. Si creino 4 produttori (ognuno produca 5 messaggi) e 2
consumatori (ognuno consumi 10 messaggi). Prima di ogni consumazione, i
consumatori dovranno attendere un secondo usando la primitiva `sleep()`.
I messaggi dovranno contenere un valore intero casuale, che i
consumatori stamperanno a video.

![image](/images/ambiente_locale/code_messaggi/controllo_di_congestione.png)
