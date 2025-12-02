# Pipeline di code, con probabilità di modifiche

Si realizzi un'applicazione **multiprocesso** di tipo Client-Server,
utilizzando **code di messaggi UNIX**. In aggiunta a 3 processi Client e
al processo Server (vedi figura), il programma dovrà avere un terzo tipo
di processo **Iniettore**, che corromperà i messaggi in transito tra
Client e Server.

I messaggi dovranno contenere una coppia di campi (oltre al campo
"tipo"): un vettore di caratteri, e un valore intero. Ciascun Client
dovrà inserire nel vettore di caratteri<sup>[\[1\]](#footnote1)</sup> rispettivamente le stringhe
"CLIENT1", "CLIENT2", "CLIENT3" (ripetendo la stessa stringa su tutti i
messaggi inviati dallo stesso Client); e dovrà inserire nel campo intero
la lunghezza della stringa (in questo caso, il valore "7" poiché vi sono
7 caratteri). Si inviino 3 messaggi per ogni Client.

Il processo Iniettore dovrà consumare i messaggi da una prima coda di
messaggi. L'Iniettore dovrà decidere, in maniera casuale<sup>[\[2\]](#footnote2)</sup>, se
inoltrare il messaggio nella seconda coda di messaggi in maniera intatta
(70% di probabilità) oppure corrotta. Nel caso si debba corrompere il
messaggio, il processo dovrà aggiungere un valore casuale al campo
intero (10% di probabilità) oppure inserire il carattere '\\0' in una
posizione a piacere della stringa (20% probabilità).

Il processo Server dovrà verificare l'integrità del messaggio,
controllando che la lunghezza della stringa<sup>[\[3\]](#footnote3)</sup> coincida con il valore
intero incluso nel messaggio. In caso affermativo, il processo Server
dovrà stampare il messaggio su terminale. In caso negativo, dovrà
stampare un messaggio di errore.

![image](/images/ambiente_locale/code_messaggi/pipeline_di_code_con_probabilita_di_modifiche.png)

<a name="footnote1">[1]</a>: Utilizzare `strcpy(x.stringa, “MSG1")` oppure
    `strcpy(x.stringa, y.stringa)` (definita in `<string.h>`) per
    scrivere nel vettore di caratteri.

<a name="footnote2">[2]</a>: Si generi un numero casuale tra 0 e 99 mediante `rand() % 100`, e
    si valuti (con una sequenza di `if`) se il numero ricade
    rispettivamente negli intervalli tra 0-69, tra 70-79, oppure tra
    80-99.

<a name="footnote3">[3]</a>: Utilizzare la funzione `strlen(x.stringa)` per ottenere la
    lunghezza della stringa.
