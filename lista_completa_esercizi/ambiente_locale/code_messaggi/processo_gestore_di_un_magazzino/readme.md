# Processo gestore di un magazzino

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso**,
basata su **code di messaggi UNIX**, per simulare la gestione di un
magazzino. L'applicazione dovrà istanziare tre tipi di processi:

-   Un processo **Gestore**, che gestisce la risorsa "magazzino" in
    memoria, definita dal vettore:

        typedef struct {
            int costo;
            int quantita;
        } Magazzino[10];

    Ciascuna tipologia di prodotto è identificata da un codice intero
    tra 0 e 9 (la **posizione nel vettore**).

-   Un gruppo di processi **Clienti**, che inviano due tipi di richieste
    verso il Gestore: (1) **lettura dello stato del magazzino** (il
    Gestore risponde con un messaggio contenente l'intero vettore del
    magazzino); (2) **acquisto di un prodotto** (il Cliente specifica
    nella richiesta il codice del prodotto; il Gestore decrementa la
    quantità disponibile, e risponde con un messaggio indicante la
    quantità di prodotto acquistata).

-   Un gruppo di processi **Fornitori**, che inviano richieste verso il
    Gestore per **rifornire il magazzino** (il Fornitore indica il
    prodotto e la quantità fornita; il Gestore incrementa in base alla
    quantità indicata).

I processi Clienti e Fornitori dovranno inviare le richieste al Gestore
usando la stessa coda. Il processo Gestore invia risposte ai Clienti
tramite una seconda coda. Le richieste inviate dai Clienti dovranno
contenere un **campo aggiuntivo** con il PID del processo richiedente;
il Gestore dovrà usare il PID nel campo "tipo" del messaggio di
risposta, per fare in modo che la risposta giunga al Cliente che ha
originato quella risposta. Tutti i processi dovranno stampare tutti i
messaggi inviati e ricevuti.

Si creino 3 processi Clienti e 2 processi Fornitori. Ogni Cliente dovrà
generare una richiesta di lettura e 2 richieste di acquisto; prima di
inviare una nuova richiesta, il Cliente deve ricevere la risposta alla
richiesta precedente e attendere 1 secondo. Ogni Fornitore dovrà
effettuare una sequenza di 2 forniture, attendendo 2 secondi prima di
ogni fornitura. Si scelga casualmente i valori nelle richieste, e i
valori iniziali del vettore (tra 1 e 5). Il Gestore dovrà terminare dopo
aver servito le 13 richieste dagli altri processi.

![image](/images/ambiente_locale/code_messaggi/processo_gestore_di_un_magazzino.png)
