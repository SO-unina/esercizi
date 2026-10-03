# Produttore/consumatore con vettore di stato, con monitor

Si realizzi in linguaggio C/C++ un'applicazione **multithread** per la
simulazione di un magazzino per lo stoccaggio di merci. L'applicazione è
costituita da 20 thread, 10 **fornitori** e 10 **clienti**. Il
magazzino è rappresentato da un vettore di 100 elementi, ognuno
contenente la seguente struttura **scaffale**:

    typedef struct {
        unsigned int id_fornitore;
        unsigned int stato; 
    } scaffale;

dove `id_fornitore` è l'identificativo dell'ultimo thread fornitore che
ha usato lo scaffale (inizialmente 0; l'identificativo è un intero
passato come argomento alla creazione del thread), e stato indica se lo
scaffale è **libero** (0), **occupato** (1), o **in uso** (2), ovvero
correntemente usato da un fornitore o cliente. Una variabile
`livello_scorte` indica il numero di prodotti presenti nel magazzino
(livello max: 100 prodotti, inizialmente 0).

Ognuno dei thread (fornitori e clienti) effettua 15 accessi al
magazzino. Ad ogni accesso, il fornitore stabilisce se c'è spazio per
effettuare la fornitura, in base al `livello_scorte`, altrimenti si
sospende in attesa che qualche cliente liberi spazio. In seguito, pone
in uso il primo scaffale libero che trova ed effettua la fornitura
ponendo a occupato lo stato dello scaffale e il proprio identificativo
in `id_fornitore` (si simuli una durata di 1 secondo per la fornitura,
utilizzando una `sleep()`). Infine, aggiorna il `livello_scorte`.

Allo stesso modo, il cliente stabilisce se ci sono prodotti, in base al
`livello_scorte`, altrimenti si sospende in attesa di fornitori. In
seguito, pone in uso il primo scaffale occupato che trova ed effettua
l'acquisto (si simuli una durata dell'acquisto di 1 secondo). Infine
pone a libero lo stato dello scaffale e aggiorna il `livello_scorte`.
Il magazzino e la variabile `livello_scorte` sono variabili condivise
tra i thread, e l'accesso a tali variabili deve essere disciplinato
attraverso il costrutto **Monitor**, realizzato con i Pthreads
(**mutex** e **condition variables**) con semantica
**signal-and-continue**.

**Nota didattica**: la versione originale di questo esercizio era basata
su processi e sul monitor di Hoare (semantica signal-and-wait), in cui
l'attesa poteva essere realizzata con un semplice `if`. Con la semantica
signal-and-continue il thread risvegliato non entra immediatamente nel
monitor, quindi la condizione logica va ricontrollata in un ciclo
`while` attorno alla wait: in questo modo la soluzione resta corretta
anche se un altro thread modifica lo stato prima del rientro nel
monitor.

I thread fornitori e clienti sono generati dal programma principale
attraverso la primitiva `pthread_create()`. Una volta generati i
thread, il programma principale ne attende la terminazione con
`pthread_join()` e termina a sua volta.
