# Produttore/consumatore con vettore di stato, con monitor

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso** per la
simulazione di un magazzino per lo stoccaggio di merci. L'applicazione è
costituita da 20 processi, 10 **fornitori** e 10 **clienti**. Il
magazzino è rappresentato da un vettore di 100 elementi, ognuno
contenente la seguente struttura **scaffale**:

    typedef struct {
        unsigned int id_fornitore;
        unsigned int stato; 
    } scaffale;

dove `id_fornitore` è il PID dell'ultimo fornitore che ha usato lo
scaffale (inizialmente 0), e stato indica se lo scaffale è **libero**
(0), **occupato** (1), o **in uso** (2), ovvero correntemente usato da
un fornitore o cliente. Una variabile globale `livello_scorte` indica il
numero di prodotti presenti nel magazzino (livello max: 100 prodotti,
inizialmente 0).

Ognuno dei processi (fornitori e clienti) effettua 15 accessi al
magazzino, uno ogni secondo. Ad ogni accesso, il fornitore stabilisce se
c'è spazio per effettuare la fornitura, in base al `livello_scorte`,
altrimenti si sospende in attesa che qualche cliente liberi spazio. In
seguito, pone in uso il primo scaffale libero che trova ed effettua la
fornitura ponendo a occupato lo stato dello scaffale e il proprio PID in
`id_fornitore` (si simuli una durata di 2 secondi per la fornitura,
utilizzando una `sleep()`). Infine, aggiorna il `livello_scorte`.

Allo stesso modo, il cliente stabilisce se ci sono prodotti, in base al
`livello_scorte`, altrimenti si sospende in attesa di fornitori. In
seguito, pone in uso il primo scaffale occupato che trova ed effettua
l'acquisto (si simuli una durata dell'acquisito di 2 secondi). Infine
pone a libero lo stato dello scaffale, pone a 0 la variabile
`id_fornitore`, e aggiorna il `livello_scorte`. Il magazzino e la
variabile `livello_scorte` sono variabili condivise, e l'accesso a tali
variabili da parte dei processi deve essere disciplinato attraverso il
costrutto **Monitor**.

I processi fornitori e clienti sono generati dal programma principale
attraverso la primitiva `fork()`. Una volta generati i processi, il
programma principale ne attende la terminazione e termina a sua volta.
