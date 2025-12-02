# Pipeline di code, con filtraggio

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso** basata
su **code di messaggi UNIX**, secondo lo schema in **pipeline**
illustrato di seguito e in figura.

I processi detti **generatore** (3 in totale) dovranno generare messaggi
contenenti una stringa di massimo 10 caratteri, un array di 2 interi, e
un ulteriore variabile intera. Ogni generatore invierà 2 messaggi. La
stringa dovrà essere assegnata utilizzando caratteri casuali<sup>[\[1\]](#footnote1)</sup>; i
valori dell'array di interi dovrà essere scelto casualmente tra 0 e 9;
la terza variabile intera dovrà essere posta a 0. Dopo aver inviato i 2
messaggi, i processi generatori dovranno terminare.

Il processo **filtro** dovrà ricevere i messaggi dai processi
generatori, e dovrà effettuare una ricerca del carattere 'x' all'interno
della stringa<sup>[\[2\]](#footnote2)</sup>. Se la stringa non contiene il carattere 'x', allora
il messaggio viene inoltrato al processo successivo; altrimenti, il
messaggio viene ignorato. Il processo filtro dovrà ripetere 6 volte la
ricezione e la ricerca, e poi terminare.

Il processo **checksum** dovrà ricevere i messaggi dal processo filtro.
Ad ogni messaggio, dovrà calcolare la somma di tutti i caratteri della
stringa e del vettore di 2 interi, e inserire il risultato nella terza
variabile intera del messaggio. Infine, dovrà inviare il messaggio
risultante al processo **visualizzatore**, il quale farà una stampa a
video di tutto il messaggio. Entrambi i processi terminano dopo aver
elaborato 6 messaggi.

![image](/images/ambiente_locale/code_messaggi/pipeline_di_code_con_filtraggio.png)

<a name="footnote1">[1]</a>: Si assegnino alla stringa valori casuali nell'intervallo \[141,
    172\], ad esempio tramite: `msg.stringa[i] = (rand() % 32) + 141`,
    con `i` compreso tra 0 e 8.Si ricordi di assegnare all'ultimo
    carattere della stringa il carattere terminatore (valore 0), con:
    `msg.stringa[9] = ‘\0’`.

<a name="footnote2">[2]</a>: Per effettuare la ricerca del carattere, è possibile utilizzare:
    `strchr(msg.stringa, ’x’);` (ritorna NULL se il carattere non è
    presente nella stringa).
