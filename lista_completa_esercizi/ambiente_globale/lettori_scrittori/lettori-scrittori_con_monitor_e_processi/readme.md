# Lettori/scrittori con monitor e processi

Si realizzi in linguaggio C/C++ un'applicazione multiprocesso per la
simulazione di un servizio meteo. L'applicazione sia costituita da 11
processi, di cui un processo, denominato nel seguito M, rappresenti il
servizio di meteo, e gli altri 10 processi rappresentino ipotetici
utenti del servizio. Il processo M detiene le informazioni di meteo
nella seguente struttura **meteo**:

    typedef struct {
        int temperatura;
        unsigned int umidita;
        unsigned short pioggia;
    } meteo;

dove la variabile "temperatura" è un intero compreso tra -50 e +50, la
variabile "umidita" è espressa in percentuale tra 0 e 100, e la
variabile "pioggia" indica se il tempo è piovoso (1) o non piovoso (0).

Il processo M genera in maniera casuale<sup>[\[1\]](#footnote1)</sup> ogni 2 secondi le
informazioni meteo, memorizzandole nella struttura. M termina dopo aver
effettuato 20 scritture. La scrittura deve avvenire in mutua esclusione.

Ognuno dei 10 processi utente effettuano un'operazione di lettura ogni
secondo dalla struttura "meteo", stampando a video le informazioni
lette. I processi utente terminano dopo aver effettuato 10 letture. Due
o più processi utente possono leggere contemporaneamente dalla
struttura, se non occupata da M.

La struttura "meteo" deve essere memorizzata in un segmento di memoria
condivisa, e l'accesso a tale struttura da parte di M e dei processi
utente deve essere disciplinato attraverso un **monitor**.

Il processo M e i 10 processi utente sono generati dal programma
principale attraverso le primitiva `fork`. Una volta generati i
processi, il programma principale ne attende la terminazione prima di
eliminare eventuali risorse condivise.

<a name="footnote1">[1]</a>: La generazione casuale può essere implementata con la funzione
    rand() di stdlib.h; ad esempio: `int umidita = rand() % 101`. La
    funzione rand() richiede che venga generato un seme dei numeri
    casuali attraverso la funzione `srand(time(NULL))`.
