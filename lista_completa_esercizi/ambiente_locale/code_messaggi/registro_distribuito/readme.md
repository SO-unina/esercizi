# Registro Distribuito

Si realizzi in linguaggio C/C++ la seguente applicazione basata su
**code di messaggi POSIX**.

La applicazione include 2 **processi server**. Ogni server è dotato di
una **risorsa logica**, rappresentata da una variabile intera. Al loro
avvio, ogni processo server crea una **propria mailbox**, su cui
ricevere le richieste dei client. Prima di porsi in attesa delle
richieste, i server inviano ad un **processo registro** una richiesta di
\"*bind*\", con cui inviano al registro un loro identificativo numerico
(\"1\" oppure \"2\"), e il **nome** della loro coda di messaggi POSIX
privata.

Il **processo registro** contiene al suo interno un array di stringhe,
in cui memorizza per ogni server il nome della corrispondente coda di
messaggi. Inoltre, il processo registro crea una mailbox per ricevere i
messaggi di \"*bind*\" e \"*query*\"; i messaggi di \"*result*\" vengono
inviati sulle code di risposta dedicate dei client.

I 3 **processi client** scelgono in modo casuale quale server contattare
(un numero tra \"1\" e \"2\"). Essi inviano inizialmente un messaggio di
\"*query*\" al **processo registro**, indicando il server che vogliono
contattare. In risposta, i client ricevono un messaggio che indica la
coda di messaggi con cui contattare il server. La ricezione va
effettuata su una **coda di risposta dedicata** che ogni client crea al
proprio avvio, con un nome costruito a partire dal proprio PID (es.
`/registro_risp_<pid>`), e il cui nome viene comunicato al registro
all'interno del messaggio di \"*query*\".

Infine, ogni client invia al server 3 messaggi di \"*service*\" (attendendo
1 secondo ad ogni invio), contenenti un valore intero casuale tra 0 e 10.
Ogni **processo server** riceve le richieste dei client, sovrascrivendo la
propria risorsa logica con il valore ricevuto.

Si crei un unico eseguibile, in cui un processo padre avvia tutti gli
altri processi. Il padre attende che tutti i processi client abbiano
terminato dopo aver inviato ognuno 3 messaggi. Dopodiché, il padre invia
un messaggio di \"*exit*\" al processo registro, che a sua volta invia
due messaggi di \"*exit*\" ai due server.

![image](/images/ambiente_locale/code_messaggi/registro_distribuito.png)
