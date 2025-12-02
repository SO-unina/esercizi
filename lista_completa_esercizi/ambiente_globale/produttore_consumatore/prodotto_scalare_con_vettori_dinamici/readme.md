# Prodotto scalare con vettori dinamici

Si realizzi in linguaggio C/C++ un'applicazione **multithread** per il
calcolo del prodotto scalare tra due vettori. Si introduca una struttura
**monitor** Vettore per gestire ciascun vettore:


    typedef struct {
        int * vettore;  // puntatore ad un vettore di interi, da allocare su heap
        int dimensione;
        // ... inserire altre variabili per la sincronizzazione ...
    } Vettore;

    void inizializza(Vettore * v, int dimensione);
    void inserisci_elemento(Vettore * v, int elemento);
    int preleva_elemento(Vettore * v);
    void distruggi(Vettore * v);

Il programma principale dovrà creare due istanze della struttura
Vettore, inizializzarle con la funzione `inizializza()` con dimensione
pari a 5, ed avviare un thread **Manager** e 4 thread **Worker**. Il
Manager dovrà utilizzare più volte la funzione `inserisci_elemento()`
per inserire in totale 20 valori interi (scelti a caso tra 0 e 5) nei
due vettori, ossia, dovrà inserire 10 valori in ciascun vettore. **Il
Manager dovrà alternare gli inserimenti nel primo e nel secondo
vettore.** Nel caso che un vettore sia pieno, il Manager deve
sospendersi fino a quando non si rende disponibile dello spazio nel
vettore.

I thread Worker eseguono in parallelo al Manager. Essi dovranno iterare
più volte, attendendo 1 secondo alla fine di ogni iterazione. Ad ogni
iterazione, un Worker **preleva una coppia di valori** usando la
funzione `preleva_elemento()` sui ciascuno dei due vettori. Nel caso che
un vettore sia vuoto, il Worker deve sospendersi fino a quando non si
renda disponibile un elemento nel vettore. Il Worker dovrà effettuare il
prodotto tra i due valori, e sommare il risultato ad una variabile
`somma` condivisa tra i thread. Inoltre, dovrà incrementare di 1 il
valore di una variabile `conteggio` condivisa tra i thread. Prima di
prelevare una coppia di valori, i thread Worker dovranno ispezionare la
variabile `conteggio`, e terminare nel caso essa sia già uguale a 10. Il
programma principale dovrà attendere la terminazione dei thread Manager
e Worker, e stampare il valore finale della somma.

![image](/images/ambiente_globale/produttore_consumatore/prodotto_scalare_con_vettori_dinamici.png)
