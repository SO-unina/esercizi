# Produzione di un vettore variabile di elementi

Si realizzi in linguaggio C/C++ una applicazione **multithread** basata
sullo schema del **produttore-consumatore con vettore di stato**,
utilizzando il costrutto **monitor** e la libreria **PThreads**.


    typedef struct {
        int elementi[5];
        int stato[5];   // LIBERO, IN_USO, oppure OCCUPATO
        int elementi_liberi;
        int elementi_occupati;
        // ... aggiungere altre variabili per la sincronizzazione ...
    } MonitorProdCons;

    void produci_elementi(MonitorProdCons * m, int valori[], int numero_valori);
    int consuma_elemento(MonitorProdCons * m);

Il metodo `consuma_elemento` (come nel caso base del
produttore-consumatore visto al corso) consuma un solo elemento se
disponibile; e pone il thread chiamante in attesa nel caso non vi siano
elementi disponibili nel vettore (indicati dalla variabile di stato
`elementi_occupati` nel monitor). Il metodo attende 1 secondo prima di
consumare, e stampa a video il valore consumato. Il metodo
`produci_elementi` può produrre da 1 a 5 elementi in una sola chiamata.
Il chiamante indica (tramite il parametro `numero_valori”` la quantità
di valori da inserire nel vettore. I valori da inserire sono passati
tramite lo array di ingresso `valori`. Il thread chiamante deve essere
posto in attesa nel caso non vi siano sufficienti buffer liberi nel
vettore (ossia, il parametro `numero_valori` è superiore alla variabile
di stato `elementi_liberi`, con cui contare il numero di buffer liberi).
Come nel caso base del produttore-consumatore con vettore di stato, il
metodo `produci_elementi` dovrà ispezionare il vettore di stato (ossia
identificare quanti e quali buffer sono nello stato libero, e porli
nello stato "in uso") entrando nella sezione critica del monitor.
L'inserimento degli elementi nei buffer liberi dovrà essere fatto fuori
dalla sezione critica. Si suggerisce che il metodo memorizzi le
posizioni degli elementi liberi in un array, definito come variabile
locale del metodo; e che iteri su questo array per effettuare le
produzioni. Dopo aver inserito gli elementi, il metodo dovrà stampare a
video i valori inseriti e la loro posizione, e attendere 1 secondo.

Il programma principale dovrà creare 1 thread produttore, e 3 thread
consumatori. Il produttore dovrà chiamare 5 volte il metodo
`produci_elementi`, passando la prima volta 5 valori; poi 4 valori; poi
3 valori; etc. (usando valori a piacere). I thread consumatori dovranno
ognuno chiamare 5 volte il metodo `consuma_elemento`.
