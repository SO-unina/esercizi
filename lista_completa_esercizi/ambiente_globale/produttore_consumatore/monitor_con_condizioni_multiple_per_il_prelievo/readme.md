# Monitor con condizioni multiple per il prelievo

Lo studente realizzi in linguaggio C/C++ un'applicazione
**multithread**, basata sul costrutto **monitor**, per simulare la
gestione di un servizio di spedizioni. Si utilizzino (modificandole) le
seguenti definizioni:


    typedef struct {
        int ordini[4];
      // ... aggiungere ulteriori variabili per la sincronizzazione
    } MonitorSpedizioni;

    void inserisci(MonitorSpedizioni * m, int quantita);
    void preleva(MonitorSpedizioni * m, int quantita_minima);

Il primo metodo inserisce all'interno del vettore `ordini` (da gestire
come coda circolare) il valore intero indicato come parametro di
ingresso. Se il vettore è già interamente pieno, il thread chiamante si
sospende.

Il secondo metodo preleva dal vettore `ordini` **tutti** i valori
contenuti nel vettore, **se e solo se** si verifica una delle seguenti
due condizioni (come in figura):

1.  Il vettore è interamente pieno; oppure

2.  La somma dei valori è maggiore o uguale al parametro
    `quantita_minima`.

Nel caso che nessuna delle due condizioni è verificata, il thread
chiamante si sospende. Se almeno una delle due condizioni si verifica,
il thread si sblocca, consuma tutti gli elementi e stampa a video i
valori.

![image](/images/ambiente_globale/produttore_consumatore/monitor_con_condizioni_multiple_per_il_prelievo.png)

Il programma principale dovrà creare 3 thread, ciascuno dei quali
effettuerà 5 inserimenti in un ciclo (con una attesa di 1 secondo ad
ogni iterazione del ciclo). Al primo inserimento, il thread inserisce il
valore 1; al secondo, il valore 2; al terzo, il valore 3; etc. Inoltre,
il programma principale dovrà creare un ulteriore thread che chiamerà
ripetutamente il metodo `preleva`, fin quando la somma totale di tutti i
valori consumati non raggiunga 45. Si utilizzi sempre il valore 4 per il
parametro `quantita_minima`.
