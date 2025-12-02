# Processo di interfaccia sincrono con server asincroni

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso**,
basata su **code di messaggi UNIX**, utilizzando tre tipi di processi:
**Client** (5 in totale), **Interfaccia**, e **Server** (3 in totale).

Ognuno dei processi Client invia 5 richieste di elaborazione al processo
Interfaccia. La prima richiesta deve essere inviata tramite **send
sincrona**, e le successive 4 tramite **send asincrona**. Nel realizzare
la send sincrona tra Clienti e Interfaccia, i processi dovranno
utilizzare il PID del processo Client nei messaggi, per fare in modo che
i messaggi di sincronizzazione OK-TO-SEND siano recapitati ad uno
specifico Client. Inoltre, le richieste dei Client dovranno contenere un
valore intero casuale tra 0 e 9.

Il processo Interfaccia dovrà servire un solo processo Client alla volta
(se altri Client hanno fatto REQUEST-TO-SEND, si pongono in attesa della
rispettiva OK-TO-SEND). Il processo Interfaccia deve inizialmente porsi
in attesa di una richiesta da una send sincrona, poi ricevere le
successive richieste da 4 send asincrone. Per ogni richiesta ricevuta
(sincrona o asincrona), il processo Interfaccia dovrà a sua volta
inviare la richiesta ai processi Server (tramite send asincrona), usando
**code distinte** per ognuno dei 3 Server, e usando una politica
**round-robin** (ad esempio, le prime 3 richieste, nell'ordine, saranno
inviate al primo, al secondo e al terzo Server; la quarta richiesta sarà
inviata al primo Server; la quinta richiesta sarà inviata al secondo
Server; etc.). Ad ogni richiesta, i Server dovranno stampare a video il
proprio PID, e il PID e il valore trasmesso dal richiedente.

Dopo aver ricevuto le 5 richieste, il processo Interfaccia dovrà gestire
il successivo Client, ripetendo ogni volta la stessa sequenza di passi.
Ognuno dei processi terminerà dopo aver inviato e/o ricevuto un numero
prefissato di messaggi.

![image](/images/ambiente_locale/code_messaggi/processo_di_interfaccia_sincrono_con_server_asincroni.png)
