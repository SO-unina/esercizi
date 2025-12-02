# Lettori/scrittori multipli

Si realizzi in linguaggio C/C++ il seguente programma **multithread**
basato sullo schema **lettore/scrittore**. Il programma dovrà
implementare una struttura dati `BufferLS`, in cui inserire una
variabile di tipo `int` (rappresentante un singolo buffer), e le
ulteriori variabili necessarie per la sincronizzazione. Inoltre, il
programma dovrà includere due funzioni `int Lettura(BufferLS *)` e
`void Scrittura(BufferLS *, int v)` per l'accesso al buffer. La
sincronizzazione dovrà garantire la **starvation dei soli scrittori**
(ossia, è sufficiente la mutua esclusione del monitor per sincronizzare
i thread scrittori).

Il programma principale dovrà creare 3 istanze della struttura
`BufferLS`, e 7 thread organizzati come in figura. Il primo thread dovrà
scrivere nel buffer un valore a caso tra 0 e 9, per 3 volte, attendendo
3 secondi tra una scrittura e l'altra. I thread 2 e 3 dovranno, con
periodicità di 2 secondi, per 5 volte, leggere un valore dal primo
buffer, e scrivere il valore letto su un ulteriore buffer. I thread 4,
5, 6, 7 dovranno leggere, con periodicità di 1 secondo e per 10 volte,
il valore dal relativo buffer. Ad ogni operazione di lettura e
scrittura, dovranno essere stampati a video il valore e un numero
identificativo del thread. Il programma dovrà terminare alla
terminazione di tutti i thread.

![image](/images/ambiente_globale/lettori_scrittori/lettori-scrittori_multipli.png)
