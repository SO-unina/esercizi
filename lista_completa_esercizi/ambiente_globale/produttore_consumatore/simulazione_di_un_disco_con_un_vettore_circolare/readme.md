# Simulazione di un disco con un vettore circolare

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso** per la
simulazione di un algoritmo di schedulazione dei dischi. L'applicazione
deve essere costituita da un processo **Schedulatore** e da un gruppo di
5 processi **Utente**. Ogni processo **Utente** genera 5 richieste di
operazioni sul disco, che devono essere collocate in una coda circolare
di 10 elementi allocata in una memoria condivisa, per poi terminare la
sua esecuzione. Se la coda è piena, il processo **Utente** deve mettersi
in attesa che vi sia una posizione disponibile. Una richiesta deve
contenere un valore da salvare sul disco (si utilizzi il PID del
processo) e la posizione in cui salvare il valore (un valore intero
casuale tra 0 e 19):


    typedef struct {
        unsigned int posizione;
        pid_t processo;
    } richiesta;

Il processo **Schedulatore** preleva le richieste dalla testa della coda
circolare (ossia applicando una politica FIFO). Per simulare la
operazione su disco, lo Schedulatore attende per un intervallo di tempo
(tramite la primitiva `sleep()`) di durata <img src="https://render.githubusercontent.com/render/math?math=t_i = | p_i - p_{i-1} |">
secondi, dove <img src="https://render.githubusercontent.com/render/math?math=p_i"> rappresenta la posizione sul disco della <img src="https://render.githubusercontent.com/render/math?math=i">-esima
operazione, assumendo <img src="https://render.githubusercontent.com/render/math?math=p_0=0">. Dopo aver atteso <img src="https://render.githubusercontent.com/render/math?math=t_i"> secondi, lo
Schedulatore salva il valore indicato nella richiesta alla posizione
<img src="https://render.githubusercontent.com/render/math?math=p_i"> di un array rappresentante il disco (da allocare come variabile
automatica). Lo **Schedulatore** termina dopo aver servito 25 richieste
provenienti dai processi **Utente**.

Si sincronizzi l'accesso alla coda circolare e ai relativi puntatori
`testa` e `coda` tramite **semafori POSIX anonimi process-shared**
(directory `soluzione_semafori`): la coda e i `sem_t` sono collocati in
una **memoria condivisa POSIX** creata con `shm_open` + `ftruncate` +
`mmap`, i semafori sono inizializzati con `sem_init` e, al termine, il
programma principale li distrugge con `sem_destroy` e rimuove l'oggetto
di shared memory con `shm_unlink`.

In una versione alternativa dell'esercizio (directory
`soluzione_monitor`), si realizzi invece un'applicazione
**multithread**: **Utente** e **Schedulatore** sono thread, la coda
circolare è una normale struttura condivisa tra i thread, e gli accessi
sono sincronizzati attraverso il costrutto **Monitor**, realizzato con i
Pthreads (**mutex** e **condition variables**) con semantica
**signal-and-continue**: le attese sulle condition variables vanno
quindi realizzate con cicli `while` che ricontrollano la condizione
logica al risveglio (a differenza del monitor di Hoare, in cui bastava
un semplice `if`). In questa versione, come valore da salvare sul disco
si utilizzi l'identificativo del thread **Utente** (un intero passato
come argomento alla creazione del thread) al posto del PID, e si simuli
l'operazione su disco con un'attesa di <img src="https://render.githubusercontent.com/render/math?math=t_i"> *decimi* di secondo
(tramite la primitiva `usleep()`).

I processi **Utente** e **Schedulatore** sono generati da un unico
programma principale attraverso la primitiva `fork()` (nella versione
con monitor, i thread sono creati tramite la primitiva
`pthread_create()`). Una volta generati i processi (o i thread), il
programma principale ne attende la terminazione e termina a sua volta.

![image](/images/ambiente_globale/produttore_consumatore/simulazione_di_un_disco_con_un_vettore_circolare.png)
