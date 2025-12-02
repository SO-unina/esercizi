# Allocatore di memoria

Si realizzi in linguaggio C/C++ una struttura dati **monitor** che funga
da allocatore di memoria per applicazioni **multithread** (usando
PThreads). Essa deve realizzare la seguente interfaccia (o equivalente
C++):


    typedef struct {
        char memoria[10];       // array di caratteri da allocare
        int stato[10];      // array di stato dei caratteri da allocare
        ...             // aggiungere qui variabili per la sincronizzazione
    } Allocatore;

    void AllocInit(Allocatore * a);         // inizializza la struttura dati
    char * getMemoria(Allocatore * a, int n);               // alloca n caratteri
    void releaseMemoria(Allocatore * a, char * blocco, int n);  // dealloca n caratteri

La struttura dati include un array di 10 caratteri da allocare, un array
di variabili di stato per ognuno dei caratteri da allocare (libero
oppure occupato) ed ulteriori variabili per la sincronizzazione a cura
dello studente. Il metodo `getMemoria()` alloca `n` elementi liberi
contigui dell'array (marcandoli occupati), e ritorna un puntatore al
primo carattere allocato<sup>[\[1\]](#footnote1)</sup>. Questo metodo seleziona gli elementi da
allocare secondo la strategia first-fit (si alloca il primo blocco di
caratteri sufficientemente grande). Se non si trova un blocco
sufficientemente grande da allocare, il thread richiedente deve essere
sospeso finché non si libera memoria a sufficienza. Il metodo
`releaseMemoria()` libera `n` elementi contigui a partire dalla
posizione `blocco` (marcandoli liberi). Inoltre, tutti i thread in
attesa devono essere risvegliati<sup>[\[2\]](#footnote2)</sup> e controllare se si è reso
disponibile un blocco sufficientemente grande, mettendosi di nuovo in
attesa se questo non accade.

Si sviluppi inoltre un programma che utilizzi l'allocatore. Esso deve
istanziare 5 thread, ciascuno dei quali alloca un blocco di dimensione
casuale (da 1 a 3 caratteri), attende 3 secondi e rilascia il blocco.
Una volta generati i thread (come joinable), il programma principale ne
attende la terminazione, e termina a sua volta.

<a name="footnote1">[1]</a>: Utilizzando ad esempio la sintassi `&memoria[5]` per ritornare un
    puntatore al sesto elemento.

<a name="footnote2">[2]</a>: Utilizzando la primitiva `pthread_cond_broadcast()` oppure la
    primitiva `pthread_cond_signal()`.
