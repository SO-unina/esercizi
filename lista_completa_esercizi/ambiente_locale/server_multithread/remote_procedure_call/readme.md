# Remote Procedure Call

Si realizzi in linguaggio C/C++ una applicazione client-server basata su
code di messaggi POSIX, secondo il seguente schema, detto di **"Remote
Procedure Call" (RPC)**.

In RPC, il Client chiama una funzione, che viene eseguita da un processo
Server. Il Client contiene al suo interno una funzione "fittizia"
(proxy), con lo stesso nome e parametri di ingresso/uscita della
funzione da eseguire nel Server. La funzione fittizia non esegue nessuna
elaborazione, ma invia al Server un messaggio che gli indica la funzione
da eseguire (tramite un apposito campo della struttura del messaggio), i
parametri di ingresso, ed il PID del processo Client. Dopo l'esecuzione
della funzione, il Client riceve dal Server un messaggio di risposta con
il parametro di uscita, ed un valore di errore in caso di problemi.

Il processo Server crea al suo interno un gruppo di thread "Worker", che
si pongono tutti in attesa di ricevere messaggi di richiesta dalla coda.
Il thread Worker, quando preleva un messaggio, ne ispeziona il campo che
indica la funzione, per scegliere quale funzione chiamare all'interno
del Server. Nella chiamata vengono passati i parametri inclusi nel
messaggio di richiesta. Quando la funzione chiamata ha terminato, il
valore di ritorno viene inserito nel messaggio di risposta. Il messaggio
di risposta viene inviato su una coda di risposta **dedicata al processo
Client** che ha effettuato la chiamata: ogni Client crea una propria
coda di risposta, il cui nome contiene il proprio PID, e il Worker la
individua a partire dal PID incluso nel messaggio di richiesta.

Nel processo Server sono fornite 3 funzioni RPC:

-   `void produci(int valore)`: inserisce il valore all'interno di un
    buffer (singolo).

-   `int consuma()`: preleva dal buffer il valore.

-   `void produci_con_somma(int val1, int val2, int val3)`: inserisce
    nel buffer la somma `val1+val2+val3`.

Il processo Client crea due processi figli, un Produttore e un
Consumatore. Il Produttore chiama 1 volta la funzione
`produci_con_somma`, e 2 volte la funzione `produci`. Il Consumatore
chiama 3 volte la funzione `consuma`.

![image](/images/ambiente_locale/server_multithread/remote-procedure-call.png)
