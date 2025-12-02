# Lettori/scrittori con versioning

Si realizzi in linguaggio C o C++ un programma **multithread** (con la
libreria PThreads) basato sul costrutto Monitor, che realizzi la
cooperazione tra thread usando una variante dello schema
**Lettori-Scrittori** descritta di seguito. Gli scrittori aggiornano
periodicamente una variabile condivisa di tipo intero, assegnandogli un
valore casuale tra 0 e 99. Come nello schema tradizionale, gli scrittori
non possono scrivere se altri lettori o scrittori stanno accedendo alla
variabile, e i lettori non possono leggere se altri scrittori stanno
accedendo, ma più lettori possono leggere in parallelo. Inoltre, sarà
necessario utilizzare un algoritmo con **possibilità di starvation sia
per i lettori sia per gli scrittori**.

A differenza dello schema tradizionale, **i lettori procedono nella
lettura di un dato soltanto se la variabile è stata modificata rispetto
alla lettura precedente, altrimenti i lettori devono attendere fino a
quando la variabile sia modificata da uno scrittore**. Quindi, i thread
dovranno condividere, oltre alla variabile intera menzionata, anche
un'altra variabile intera (di nome "versione", inizialmente pari a 0)
che gli scrittori andranno a incrementare al termine di ogni scrittura.
I lettori, oltre ad attendere che la variabile non sia utilizzata da
alcuno scrittore, dovranno anche **attendere nel caso che la versione
corrente del dato non sia maggiore della versione del dato al momento
della precedente lettura**. Per realizzare questo schema, è necessario
che i thread lettori memorizzino, subito dopo una lettura, il valore
della versione, per poterlo confrontare con la versione al momento della
lettura successiva.

Il programma dovrà istanziare 3 thread lettori, ciascuno dei quali dovrà
attendere un numero di secondi casuale (tra 1 e 3) prima di iniziare un
ciclo di 5 letture. Inoltre, il programma dovrà istanziare 2 thread
scrittori che effettueranno ciascuno 10 scritture, attendendo un secondo
tra una scrittura e la successiva. Le funzioni `leggi()` e `scrivi()`
del monitor dovranno stampare a video il valore del dato e la sua
attuale versione.


    struct MonitorLettScritt {
        int variabile;  // variabile condivisa (tra 0 e 99)
        int versione;   // variabile che indica il numero di scritture effettuate
    ...             // aggiungere qui variabili per la sincronizzazione
    };

    void leggi(struct MonitorLettScritt * m);
    void scrivi(struct MonitorLettScritt * m, int nuovovalore);
