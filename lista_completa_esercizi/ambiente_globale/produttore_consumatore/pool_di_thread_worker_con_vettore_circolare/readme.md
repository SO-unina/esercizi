# Pool di thread worker, con vettore circolare

Si realizzi in linguaggio C/C++ un **monitor** che implementi la
gestione di un **pool di thread** mediante la libreria PThreads. Per
pool di thread si intende un gruppo di thread che vengono allocati
subito alla creazione del pool, e che attendono l'arrivo di **task**
(compiti) da eseguire **in parallelo**. L'interfaccia da implementare (o
suo equivalente C++) è la seguente:

    typedef struct {
        int operand_1;
        int operand_2;
    } Task;

    typedef struct {
        Task tasklist[MAXTASKS];        // array con la lista di compiti da eseguire
        pthread_t pool[NUMTHREADS]; // identificativi dei thread del pool
        ...                 // aggiungere qui variabili per la sincronizzazione
    } ThreadPool;

    void ThreadPoolInit(ThreadPool * t);            // alloca i thread e li pone in attesa
    void * ThreadPoolExecTask(void * p);            // funzione eseguita da ogni thread
    void ThreadPoolPutTask(ThreadPool * t, Task task);  // richiede l'aggiunta di un nuovo task
    void ThreadPoolTerminate(ThreadPool * t);       // termina tutti i thread

L'array `tasklist` rappresenta la lista dei task da eseguire, e deve
essere gestito come un buffer circolare. L'array `pool` dovrà contenere
gli identificativi dei thread del pool. La funzione `ThreadPoolInit()`
crea i thread mediante la funzione `pthread_create()`, e fa in modo che
essi eseguano la funzione `ThreadPoolExecTask()` con in ingresso un
puntatore a `ThreadPool`. Quest'ultima funzione mette i thread in attesa
che la lista `tasklist` contenga almeno un task da eseguire: quando un
task si rende disponibile, la funzione `ThreadPoolExecTask()` preleva un
task da `tasklist`, stampa a video la somma degli operandi e si pone in
`sleep` per 1 secondo (questi passi sono ripetuti in un ciclo infinito).
La funzione `ThreadPoolPutTask()` inserisce un nuovo task all'interno di
`tasklist`; se la lista è piena, il chiamante di `ThreadPoolPutTask()`
deve essere messo in attesa che un thread prelevi un task dalla lista
(ossia, la lista diventi non-piena). La funzione `ThreadPoolTerminate()`
termina forzatamente i thread del pool mediante la primitiva
`pthread_cancel()`<sup>[\[1\]](#footnote1)</sup>. Si assuma `MAXTASKS=10` e `NUMTHREADS=5`.

Si implementi un programma che istanzi un pool di thread ed invii 20
task. Dopo aver atteso un tempo sufficientemente lungo (es., 10
secondi), il programma principale termina il pool di thread ed esce.

<a name="footnote1">[1]</a>: Prototipo: `int pthread_cancel(pthread_t thread);`
