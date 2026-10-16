## Esercizio. Problema Produttori/Consumatori con coda di messaggi sincrona

*Scrivere un’applicazione concorrente che implementi il problema dei Produttori/Consumatori singolo buffer, utilizzando un protocollo di scambio di messaggi con invio sincrono realizzato a partire da primitive asincrone e con una comunicazione indiretta.
Il programma crei dei processi che agiscano da produttori e consumatori, utilizzando una coda di messaggi per produrre e consumare valori di tipo char[]. La sincronizzazione tra produttori e consumatori deve avvenire tramite l'utilizzo delle primitive di mq_send() e mq_receive(), realizzando la sincronia con un protocollo di rendezvous su due code di servizio POSIX*

Analizzare il file [procedure.c](procedure.c) in cui vengono implementate le funzioni di produzione e consumazione, e il file [p_c_msg.c](p_c_msg.c) dove vengono utilizzate le chiamate di sistema necessarie per implementare il protocollo sincrono.

Compilare ed eseguire il codice:

```console
$ make
$ ./start
sono il produttore. Il mio pid 53623
sono il figlio consumatore. Il mio pid 53624
MESSAGGIO INVIATO: <www.unina.it>
MESSAGGIO RICEVUTO: <www.unina.it>
Figlio n.ro 53624 e' morto con status= 0
Figlio n.ro 53623 e' morto con status= 0
```





