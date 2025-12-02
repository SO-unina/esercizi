# Server con funzione di scrittore

Si realizzi in linguaggio C/C++ una applicazione **multiprocesso** per
simulare il tracciamento dei voli in arrivo e in partenza in un
aereoporto. L'applicazione dovrà avviare un gruppo di 4 processi
denominati **Gate** (un processo per ogni porto di imbarco), ognuno dei
quali avrà la responsabilità di inviare ad un processo **Aggiornatore**
un avviso ogni volta che un nuovo volo è in arrivo oppure è in partenza
dal porto di imbarco. Gli avvisi dovranno essere inviati tramite una
**coda di messaggi UNIX**. Gli avvisi dovranno contenere il numero del
porto di imbarco (da 1 a 4), il codice del volo (un intero casuale tra 0
e 99), e l' indicazione del tipo di volo (un intero casuale pari a 0 se
il volo è in arrivo, 1 se in partenza). Ogni processo **Gate** dovrà
generare 3 avvisi, attendendo un tempo casuale (compreso tra 1 e 3
secondi) tra un avviso e l'altro, ed infine terminare.

Al ricevere di un avviso, il processo **Aggiornatore** dovrà scrivere il
codice del volo all'interno della seguente struttura dati, che dovrà
contenere, per ogni porto di imbarco, il codice e il tipo del volo
previsto sul porto di imbarco. La struttura va allocata in una **memoria
condivisa UNIX**.

    typedef struct {
        int codice; int tipo;       // PARTENZA = 0, ARRIVO = 1
    } gate;

    typedef struct {
        gate lista[4];
    } aereoporto;

Infine, una coppia di processi **Display** dovrà periodicamente (una
volta ogni secondo, per un totale di 10 iterazioni) leggere la struttura
dati condivisa, e stampare a video lo stato dei porti di imbarco. Si
utilizzino dei **semafori UNIX** per gestire la sincronizzazione tra
**Aggiornatore** e **Display**.

![image](/images/ambiente_locale/code_messaggi/server_con_funzione_di_scrittore.png)
