# Produttore-consumatore con priorità

Si implementi in linguaggio C/C++ lo schema **produttore-consumatore a
priorità** illustrato di seguito. Occorre che vi siano 2 tipi di
thread produttori, indicati rispettivamente come **produttori ad alta
priorità e produttori a bassa priorità**. Come nel classico problema del
produttore-consumatore, i produttori di entrambi i tipi non possono
produrre finché non vi è un buffer disponibile. Inoltre, è necessario
implementare il seguente vincolo: **un produttore a bassa priorità può
produrre solo se non vi è alcun produttore ad alta priorità in attesa di
un buffer libero**.

Lo schema deve essere basato sul costrutto **monitor** (con semantica
**signal-and-continue**), realizzato con i Pthreads: un **mutex** per la
mutua esclusione del monitor ed un opportuno numero di **condition
variables** per gestire i vincoli del problema. Poiché la semantica è
signal-and-continue, ogni attesa su una condition variable va realizzata
con un ciclo `while` che ricontrolla la condizione logica al risveglio.
Si consideri il caso di un **vettore circolare di buffer** di tipo
intero, gestito con due puntatori logici testa e coda, e i seguenti
prototipi:


    typedef struct {
        int buffer[3];
        int testa; int coda;
        pthread_mutex_t mutex;      // mutex del monitor
        pthread_cond_t not_full_prio1;  // condition variables
        pthread_cond_t not_full_prio2;
        pthread_cond_t not_empty;
        // ... contatori dei thread in attesa ...
    } PriorityProdCons; 


    void inizializza_prod_cons(PriorityProdCons * p);
    void produci_alta_prio(PriorityProdCons * p, int id, int val);
    void produci_bassa_prio(PriorityProdCons * p, int id, int val);
    int consuma(PriorityProdCons * p, int id);
    void rimuovi_prod_cons(PriorityProdCons * p);

Il programma dovrà istanziare 1 thread produttore ad alta priorità, 3
thread produttori a bassa priorità, ed 1 thread consumatore, ciascuno
identificato da un intero passato come argomento alla creazione del
thread. Il thread produttore ad alta priorità invoca per 3 volte il
metodo `produci_alta_prio`, il quale produce un valore casuale tra 0 e
12, attendendo 2 secondi tra una invocazione e la successiva. I thread
produttori a bassa priorità invocano per 3 volte il metodo
`produci_bassa_prio`, il quale produce un valore casuale tra 13 e 25,
attendendo 1 secondo tra le invocazioni. Il thread consumatore consuma
e stampa a video un elemento invocando consuma per 12 volte, e
attendendo 1 secondo tra le invocazioni. Il programma principale attende
la terminazione dei thread con `pthread_join()` per poi terminare a sua
volta.
