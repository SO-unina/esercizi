# Due threads, coda sincrona condivisa

Si realizzi in linguaggio C/C++ un programma basato su **code di
messaggi POSIX**, che generi tre processi Client ed un processo Server. I
Client dovranno inviare messaggi al Server con una coda condivisa
(identificata da un nome, es. `/richieste`) attraverso una **send
sincrona**, da implementare a partire dalle
primitive `mq_send()` e `mq_receive()`. Il processo Server dovrà istanziare
due thread che, parallelamente, dovranno entrambi prelevare messaggi di
richiesta dalla coda condivisa. Ad esempio, entrambi i thread dovranno
effettuare delle receive sulla coda condivisa; il thread che preleverà
un messaggio in arrivo è scelto a discrezione del sistema operativo.

Quando un thread riceve una `request to send`, esso dovrà rispondere con
una `ok to send` al Client che ha generato la richiesta; lo stesso
thread dovrà poi prelevare ed elaborare il messaggio che il Client
invierà a seguito della `ok to send`. **Entrambi i thread dovranno usare
la stessa coda condivisa delle richieste**. Per evitare interferenze fra
i thread, si includa all'interno di ogni messaggio il PID del Client
(come normale campo intero della struttura del messaggio), e due valori
interi tra 0 e 10, scelti casualmente. Ogni Client dovrà creare due
**code dedicate**, i cui nomi sono costruiti a partire dal proprio PID:
una per ricevere le `ok to send` (es. `/risposte_<pid>`) e una su cui
inviare il messaggio successivo alla `ok to send` (es. `/dati_<pid>`);
il thread che preleva la `request to send` ricava dal PID i nomi delle
code dedicate di quel Client. In questo modo, i messaggi contenenti uno
stesso PID verranno gestiti dallo
stesso thread. Alla ricezione di un messaggio, il thread dovrà stampare
il PID del Client che ha generato il messaggio e la somma dei due valori
interi.

Ogni Client dovrà generare 4 messaggi e attendere le due corrispondenti
risposte, per poi terminare. Ciascuno dei due thread del Server dovrà
elaborare 6 messaggi, per poi terminare. Il processo Server dovrà
terminare quando entrambi i suoi thread hanno terminato. Il codice dei
Client e del Server deve risiedere in **due eseguibili distinti**. Si
crei inoltre un terzo eseguibile che avvii gli altri due.

![image](/images/ambiente_locale/server_multithread/due_threads_coda_sincrona_condivisa.png)
