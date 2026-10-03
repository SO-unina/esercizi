# Due threads, con buffer condiviso

Si realizzi in linguaggio C/C++ una applicazione basata su **code di
messaggi POSIX** che simuli la raccolta di dati da sensori ambientali. Il
sensore è controllato da un processo Server, il quale avvia due thread.
Il primo thread (ricevente) è in attesa di ricevere in ingresso dei
comandi da parte di un Client da una prima coda di messaggi. Il secondo
thread (lettore) invia periodicamente dei messaggi con le letture del
sensore, su una seconda coda di messaggi.

Inizialmente, il Client invia al Server un messaggio di
`INIZIO LETTURA`, contenente: (1) l'indicazione del comando in un campo
intero della struttura del messaggio; (2) il nome della coda (una
stringa che inizia con il carattere '/') su cui ricevere
le letture del sensore. **È onere del Client creare la coda per le
letture con `mq_open` e passarne il nome al Server**. Ricevuto il
comando, il thread ricevente scrive il nome della coda su una
variabile condivisa di tipo stringa (in memoria globale oppure dinamica,
inizialmente impostata alla stringa vuota). Il thread lettore dovrà
leggere in un ciclo (con
pause di un secondo) la variabile condivisa. Se il thread lettore trova
una stringa non vuota nella variabile condivisa, esso dovrà inviare
una lettura del sensore (un valore intero casuale tra 0 e 10) sulla coda
delle letture indicata dal Client.

Dopo il comando di inizio, il Client deve mettersi in attesa di ricevere
i dati sulla coda delle letture. Il Client dovrà effettuare 5 ricezioni,
poi inviare un messaggio di `FINE LETTURA` al Server. Il thread
ricevente dovrà impostare alla stringa vuota la variabile condivisa, per
indicare al
thread lettore di interrompere l'invio delle letture. **È onere del
Client rimuovere la coda per le letture con `mq_close` e `mq_unlink`
dopo aver inviato
il comando di interruzione**. In totale, i processi Client e Server
faranno 3 cicli di `INIZIO-FINE LETTURA`. Il Client ricrea una nuova
coda delle letture ad ogni ciclo, costruendo il nome della coda a
partire dai caratteri 'a', 'b'
e 'c' (es. `/coda_a`, `/coda_b`, `/coda_c`), in modo da usare un nome
diverso ad ogni ciclo. È richiesto che il codice
del Client e del Server sia in eseguibili distinti. Alla fine del terzo
ciclo, il thread ricevente rimuove la coda dei comandi (con `mq_close` e
`mq_unlink`) e forza la
terminazione del processo Server.

![image](/images/ambiente_locale/server_multithread/due_threads_con_buffer_condiviso.png)
