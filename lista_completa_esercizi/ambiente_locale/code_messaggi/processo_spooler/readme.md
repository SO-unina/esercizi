# Processo spooler

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso** basata
su **code di messaggi UNIX**, in cui un processo denominato **Server**
attende di ricevere messaggi provenienti da un gruppo di processi
denominati **Client**. Il Server deve accumulare i messaggi ricevuti in
un buffer; quando saranno stati accumulati 10 messaggi, esso lì invierà
ad un terzo processo denominato **Printer**, il quale stampa a video i
messaggi ricevuti insieme ad un numero progressivo che conti i messaggi
ricevuti. I messaggi dovranno contenere il PID del processo Client che
ha generato il messaggio, e ciascun Client dovrà generare 15 messaggi,
attendendo 1 secondo tra un messaggio e l'altro. Un unico programma
principale dovrà istanziare 5 processi Client ed i processi Server e
Printer.

![image](/images/ambiente_locale/code_messaggi/processo_spooler.png)
