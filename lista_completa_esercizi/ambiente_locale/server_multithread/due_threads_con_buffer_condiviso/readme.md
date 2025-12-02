# Due threads, con buffer condiviso

Si realizzi in linguaggio C/C++ una applicazione basata su **code di
messaggi UNIX** che simuli la raccolta di dati da sensori ambientali. Il
sensore è controllato da un processo Server, il quale avvia due thread.
Il primo thread (ricevente) è in attesa di ricevere in ingresso dei
comandi da parte di un Client da una prima coda di messaggi. Il secondo
thread (lettore) invia periodicamente dei messaggi con le letture del
sensore, su una seconda coda di messaggi.

Inizialmente, il Client invia al Server un messaggio di
`INIZIO LETTURA`, contenente: (1) l'indicazione del comando nel campo
"tipo"; (2) la chiave della coda (generata con `ftok`) su cui ricevere
le letture del sensore. **È onere del Client creare la coda per le
letture con `msgget` e passarne la chiave al Server**. Ricevuto il
comando, il thread ricevente scrive la chiave della coda su una
variabile condivisa (in memoria globale oppure dinamica, inizialmente
impostata a zero). Il thread lettore dovrà leggere in un ciclo (con
pause di un secondo) la variabile condivisa. Se il thread lettore trova
un valore diverso da zero nella variabile condivisa, esso dovrà inviare
una lettura del sensore (un valore intero casuale tra 0 e 10) sulla coda
delle letture indicata dal Client.

Dopo il comando di inizio, il Client deve mettersi in attesa di ricevere
i dati sulla coda delle letture. Il Client dovrà effettuare 5 ricezioni,
poi inviare un messaggio di `FINE LETTURA` al Server. Il thread
ricevente dovrà impostare a zero la variabile condivisa, per indicare al
thread lettore di interrompere l'invio delle letture. **È onere del
Client rimuovere la coda per le letture con `msgctl` dopo aver inviato
il comando di interruzione**. In totale, i processi Client e Server
faranno 3 cicli di `INIZIO-FINE LETTURA`. Il Client ricrea una nuova
coda delle letture ad ogni ciclo, usando `ftok` con i caratteri 'a', 'b'
e 'c' per creare chiavi diverse ad ogni ciclo. È richiesto che il codice
del Client e del Server sia in eseguibili distinti. Alla fine del terzo
ciclo, il thread ricevente rimuove la coda dei comandi e forza la
terminazione del processo Server.

![image](/images/ambiente_locale/server_multithread/due_threads_con_buffer_condiviso.png)
