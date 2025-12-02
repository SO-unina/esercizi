# Server con funzione di produttore

Si realizzi in linguaggio C/C++ una applicazione basata su **code di
messaggi, semafori e una memoria condivisa UNIX** per simulare lo
streaming di un flusso multimediale. Lo streaming è erogato da un
processo Server. Il Server si dovrà porre in attesa di messaggi di
richiesta da parte di un processo Client su una **coda di comandi**.
Quando il Server riceve una richiesta, dovrà allocare una **memoria
condivisa**, atta a contenere un vettore di buffer (di 4 elementi). Ogni
buffer dovrà contenere una coppia di valori interi. Si utilizzino i
**semafori** per sincronizzare l'accesso al vettore di buffer.

Dopo aver creato la memoria condivisa, il Server dovrà inviare un
messaggio al Client su una **coda di risposte**, indicando nel messaggio
di risposta la **lunghezza** della sequenza di dati che sarà prodotta
(un numero intero scelto casualmente, compreso tra 5 e 10), e la
**chiave** della memoria condivisa e dei semafori. Infine, dopo aver
inviato la risposta, il Server produrrà sul vettore di buffer una
sequenza di coppie di valori (scelti casualmente tra 0 e 3). Al termine
dello streaming, il Server dovrà attendere un messaggio di terminazione
dal Client sulla coda dei comandi, e infine rimuovere il vettore di
buffer.

Il processo Client invierà inizialmente una richiesta al Server sulla
coda dei comandi, utilizzando una **send sincrona**. Dopo aver inviato
la richiesta, dovrà attendere il messaggio di risposta. Il Client dovrà
poi collegarsi alla memoria condivisa, e consumare la sequenza di coppie
di valori (usando le chiavi ricevute dal Server). Il numero di
consumazioni dovrà essere pari alla lunghezza della sequenza indicata
dal Server nel messaggio di risposta. Il Client dovrà simulare una
consumazione della durata di un secondo, utilizzando la primitiva
sleep(), e stampare i valori ricevuti. Terminate le consumazioni, il
Client dovrà inviare un messaggio di terminazione mediante una **send
sincrona**.

È richiesto che il codice del Client e del Server sia in eseguibili
distinti. Il programma dovrà effettuare due cicli di streaming.

![image](/images/ambiente_locale/code_messaggi/server_con_funzione_di_produttore.png)
