# Lettori/scrittori con semafori, su una coppia di valori condivisa

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso** in cui
un processo scrittore e due processi lettori scambiano dati attraverso
un buffer condiviso, da allocare in una **shared memory POSIX**
(`shm_open`, `ftruncate`, `mmap`). Il
buffer dovrà contenere una coppia di variabili intere (`val_1` e
`val_2`), entrambe con valori tra 0 e 9. Lo scrittore dovrà scegliere
casualmente una coppia di valori e scriverla sulle due variabili
all'atto di una scrittura. I due lettori dovranno leggere,
rispettivamente, la prima e la seconda variabile. Si sincronizzi
l'accesso al buffer da parte dei processi facendo in modo che lo
scrittore si sospenda se dei lettori stanno effettuando una lettura, e
viceversa. Inoltre, si consenta ai lettori di poter leggere
contemporaneamente dal buffer. Si sincronizzi l'accesso utilizzando
**semafori POSIX anonimi** (`sem_t` allocati nella shared memory e
inizializzati con `sem_init` in modalità process-shared). Si simuli la
scrittura con una attesa di 1 secondo, e
la lettura con una attesa di 2 secondi, effettuando in entrambi i casi
una stampa a video. In totale, lo scrittore dovrà effettuare 5
scritture, e i lettori 5 letture.

Il programma deve essere sviluppato in tre eseguibili distinti, di cui:
(1) il primo eseguibile è eseguito da un processo padre che crea la
shared memory e i semafori, e genera tre
processi figli, ciascuno dei quali eseguirà uno degli altri eseguibili;
(2) un eseguibile per il codice dei lettori; (3) un eseguibile per il
codice dello scrittore. Gli eseguibili dei lettori e dello scrittore
accedono alla stessa shared memory aprendo con `shm_open` (senza
`O_CREAT`) il nome fisso dell'oggetto; al termine, il padre distrugge i
semafori (`sem_destroy`) e rimuove l'oggetto di shared memory
(`shm_unlink`).

![image](/images/ambiente_globale/lettori_scrittori/lettori-scrittori_con_semafori_su_una_coppia_di_valori_condivisa.png)
