# Lettori/scrittori con priorità

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso**,
basata su costrutto **monitor**, che implementi la variante dello schema
dei **lettori-scrittori** descritta di seguito. Il monitor dovrà fornire
le seguenti procedure:


    typedef struct {
        int buffer;     // risorsa condivisa
        // ... aggiungere altre variabili per la sincronizzazione ...
    } MonitorLS;

    void inizializza(MonitorLS * m);
    int lettore(MonitorLS * m);
    void scrittore_lento(MonitorLS * m, int valore);
    void scrittore_veloce(MonitorLS * m, int valore);

La funzione `lettore()` dovrà ritornare in uscita il valore nel buffer,
rispettando i normali vincoli dello schema lettori-scrittori (mutua
esclusione con gli scrittori; più lettore possono accedere
contemporaneamente). Le funzioni `scrittore_lento()` e
`scrittore_veloce()` dovranno scrivere sul buffer il valore passato come
parametro di ingresso, rispettando i normali vincoli dello schema
lettori-scrittori (mutua esclusione con i lettori e tra gli scrittori).
Gli scrittori veloci impiegano 1 secondo per effettuare la scrittura (da
simulare usando la funzione `sleep()`); gli scrittori lenti impiegano 2
secondi.

Si richiede di implementare il seguente schema, in cui la **starvation**
può verificarsi **sia sui lettori sia sugli scrittori**, e in cui **gli
scrittori veloci hanno la priorità sugli scrittori lenti**. Si usino
condition variables distinte per i due tipi di scrittori. Quando vi sono
in attesa sia scrittori veloci sia scrittori lenti, il processo uscente
(sia nel caso di un lettore sia di uno scrittore in uscita) dovrà
sbloccare uno scrittore veloce; altrimenti, in assenza di scrittori
veloci, il processo uscente dovrà sbloccare uno scrittore lento; oppure,
in assenza di scrittori in attesa, sbloccare un processo lettore.

Il programma dovrà creare 3 processi lettori, 2 processi scrittori
lenti, e 2 processi scrittori veloci. Ciascuno di essi dovrà chiamare la
corrispondente funzione del monitor per 3 volte, attendendo 3 secondi
tra le chiamate. Gli scrittori dovranno generare un valore casuale
intero tra 0 e 10 per la scrittura. I processi dovranno stampare a video
i valori letti e scritti. Il processo padre dovrà attendere la
terminazione dei figli e terminare a sua volta. È richiesto che il
codice sia contenuto all'interno di un unico eseguibile. Lo studente può
utilizzare a scelta una delle implementazioni delle primitive di base
del monitor che sono state utilizzate durante il corso, e disponibili
sulle macchine di laboratorio.
