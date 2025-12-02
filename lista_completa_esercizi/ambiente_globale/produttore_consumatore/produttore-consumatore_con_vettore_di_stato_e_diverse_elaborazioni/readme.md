# Produttore/consumatore con vettore di stato, e diverse elaborazioni

Si realizzi in linguaggio C/C++ un'applicazione **multithread** per
simulare la gestione di un magazzino. Si utilizzi il costrutto
**Monitor** per gestire un vettore di scaffali, utilizzando lo schema
del **produttore/consumatore con vettore di stato**, e basato sulle
seguenti definizioni:


    typedef struct {
        int tipo;           // tipo = 1 per elettrodomestici, tipo = 2 per arredi
        int costo;
    } prodotto;

    typedef struct {
        prodotto vettore[5];
        int stato[5];       // stato = LIBERO, OCCUPATO, IN_USO
        // ... inserire qui le variabili per la sincronizzazione ...
    } MonitorMagazzino;

    void inserisci_elettrodomestico(MonitorMagazzino *m, int costo);    // produttore
    void inserisci_arredo(MonitorMagazzino *m, int costo);      // produttore
    prodotto preleva_prodotto(MonitorMagazzino *m);             // consumatore
    prodotto preleva_prodotto_scontato(MonitorMagazzino *m);        // consumatore

I metodi `inserisci_elettrodomestico` e `inserisci_arredo` viene
chiamato da dei thread denominati `Fornitori`. Questi due metodi
inseriranno nel vettore rispettivamente un prodotto con tipo=1
(elettrodomestico) o tipo=2 (arredo). Il programma dovrà istanziare 4
`Fornitori`: 2 `Fornitori` inseriranno per 3 volte un elettrodomestico;
gli altri 2 `Fornitori` inseriranno per 2 volte un arredo. Si generi un
costo casuale tra 10 e 100. Ogni thread attenderà 1 secondo tra
produzioni consecutive.

I metodo `preleva_prodotto` e `preleva_prodotto_scontato` viene chiamato
da dei thread denominati `Negozianti`. Entrambi i metodi preleveranno
dal vettore un qualunque prodotto, indifferentemente che sia un
elettrodomestico o un arredo. Prima di ritornare un prodotto, il metodo
`preleva_prodotto_scontato` dovrà ridurre del 10% il costo del prodotto.
Il programma dovrà istanziare 1 thread che invocherà 5 volte
`preleva_prodotto`, e 1 thread che invocherà 5 volte
`preleva_prodotto_scontato`.
