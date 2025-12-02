# Vettore di stato, con più tipologie di prodotto e condition variables

Si realizzi in linguaggio C/C++ un'applicazione **multithread** che
simuli un negozio di abbigliamento, i cui articoli sono memorizzati in
un vettore di buffer gestito con il costrutto **Monitor**.


    typedef struct {
        int tipo;       // SCARPE = 1, GIACCA = 2
        int stato;      // LIBERO = 0, OCCUPATO = 1, IN USO = 2
    } buffer_articolo;

    typedef struct {
        buffer_articolo articoli[10];
        // ... inserire qui le variabili per la sincronizzazione ...
    } MonitorNegozio;

    void inserisci_scarpe(MonitorNegozio * m);
    void inserisci_giacca(MonitorNegozio * m);
    void preleva_scarpe(MonitorNegozio * m);
    void preleva_giacca(MonitorNegozio * m);

I thread dovranno sincronizzarsi utilizzando la soluzione del problema
dei **produttori/consumatori con vettore di stato**. Le funzioni
`inserisci_scarpe()` e `inserisci_giacca()` ricercano un elemento del
vettore nello stato libero, lo pongono temporaneamente in uso, stampando
a video un messaggio (posizione della produzione, tipo di elemento
prodotto), e lo pongono infine occupato. In assenza di buffer
disponibili, il thread chiamante deve essere sospeso.

La funzione `preleva_scarpe()` ricerca un elemento del vettore nello
stato occupato e contenente scarpe, consumando l'elemento (si simuli la
consumazione con una attesa di 1 secondo, e stampando a video un
messaggio). Analogamente, la funzione `preleva_giacca()` ricerca un
elemento del vettore nello stato occupato e contenente una giacca,
consumandolo. Se la ricerca non ha successo, il thread chiamante deve
essere sospeso. **È richiesto che la sospensione dei thread chiamanti
`preleva_scarpe()` sia separata dalla sospensione dei thread chiamanti
`preleva_giacca()`. Si utilizzino variabili-condition distinte**.

Il programma dovrà creare un thread produttore (che produca in totale 10
scarpe e 10 giacche, in alternanza), e due thread consumatori (il primo
consumi 10 scarpe, il secondo consumi 10 giacche).
