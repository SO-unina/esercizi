# Vettore di buffer di caratteri

Si realizzi in linguaggio C/C++ il seguente programma **multithread**
basato sullo **schema produttore/consumatore**. Il programma dovrà
implementare una struttura dati **monitor** `BufChar`, con all'interno
un vettore di buffer di tipo `char`, una variabile intera occupati, e le
ulteriori variabili necessarie per la sincronizzazione. Non è richiesto
che vi sia un vettore di stato, né che i metodi del monitor escano dalla
sezione critica durante la produzione/consumazione.

Nella produzione (metodo `produci()`), la sincronizzazione dovrà
verificare che il numero di caratteri non-occupati sia uguale o
superiore al parametro di ingresso `num_char`. In caso affermativo, il
thread dovrà scrivere su buffer (a partire dalla prima posizione
non-occupata) i caratteri dell'array in ingresso `caratteri`<sup>[\[1\]](#footnote1)</sup>, e
aggiornare la variabile occupati. In caso negativo, il thread chiamante
dovrà essere sospeso finché non si renda disponibile dello spazio.

Nella consumazione (metodo `consuma()`), la sincronizzazione dovrà
verificare che il numero di caratteri occupati sia non nullo. In caso
affermativo, il thread dovrà scrivere sull'array di ingresso `caratteri`
l'**intero contenuto** vettore buffer<sup>[\[2\]](#footnote2)</sup> a partire dalla prima
posizione, e azzerare la variabile occupati. Il chiamante del metodo
dovrà passare un puntatore a caratteri sufficientemente grande.

    typedef struct {
       char buffer[10]; // vettore di buffer, di tipo “char”
       int occupati;        // numero di caratteri occupati
       // ... aggiungere altre variabili per la sincronizzazione ...
    } BufChar;

    void inizializza(BufChar* b);
    void produci(BufChar* b, char* caratteri, int num_char);
    int consuma(BufChar* b, char* caratteri);  // ritorna numero_caratteri consumati

Nel programma principale si creino due thread (rispettivamente
produttore e consumatore). Il produttore dovrà chiamare (per 3 volte,
con periodicità di un secondo) il metodo produci, passando
rispettivamente vettori di 3, 4, e 5 caratteri (scelti a piacere, anche
non random). Il consumatore dovrà chiamare (per 2 volte, con periodicità
di 4 secondi) il metodo consuma, passando in ingresso un vettore vuoto
di 10 caratteri, e stampare a video<sup>[\[3\]](#footnote3)</sup> i caratteri che il metodo
consuma inserisce nel vettore passato in ingresso.

<a name="footnote1">[1]</a>: Per la produzione, si usi
    `strncpy(b->buffer + b->occupati, caratteri, num_char);`

<a name="footnote2">[2]</a>: Per la consumazione, si usi
    `strncpy(caratteri, b->buffer, b->occupati);`

<a name="footnote3">[3]</a>: Per la stampa, si usi
    `printf("%.*s", numero_caratteri, caratteri);`
