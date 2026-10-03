# Prelievi multipli

Si realizzi in linguaggio C/C++ un'applicazione **multithread** in
cui P produttori e C consumatori scambiano dati attraverso un buffer
circolare (politica FIFO) di N elementi di tipo intero, allocato come
struttura condivisa tra i thread.

I thread produttori produrranno **un solo elemento ad ogni
produzione**, mentre i thread consumatori consumeranno **due elementi
ad ogni consumazione**. Un produttore deve bloccarsi se il buffer a cui
tenta di accedere è pieno, finché non c'è spazio disponibile. I
consumatori possono prelevare dal buffer se ci sono almeno due elementi
disponibili; in caso contrario, i consumatori devono bloccarsi fino a
quando non ci sono abbastanza elementi nel buffer. L'accesso al buffer e
ai relativi puntatori di testa e coda deve essere disciplinato
attraverso il costrutto **Monitor**, realizzato con i Pthreads
(**mutex** e **condition variables**), con semantica
**signal-and-continue**.

**Nota didattica**: la versione originale di questo esercizio era
basata su processi e sul monitor di Hoare (semantica signal-and-wait),
in cui il produttore doveva risvegliare un consumatore *solo* in
presenza di almeno 2 elementi, e l'attesa poteva essere realizzata con
un semplice `if`. Con la semantica signal-and-continue il thread
risvegliato non entra immediatamente nel monitor, quindi la condizione
logica va ricontrollata in un ciclo `while` attorno alla wait: in
questo modo la soluzione resta corretta anche se un altro thread
modifica lo stato prima del rientro nel monitor.

Il programma dovrà istanziare 5 thread produttori, ciascuno dei quali
produrrà un elemento per 6 volte, attendendo due secondi tra una
produzione e l'altra. Inoltre, si dovranno istanziare 5 thread
consumatori, ciascuno dei quali preleverà due elementi dal buffer per 3
volte, attendendo un secondo tra una consumazione e l'altra; gli
elementi prelevati saranno stampati a video, insieme all'identificativo
del thread (passato come argomento alla creazione del thread). Una
volta istanziati i thread, tramite la primitiva `pthread_create()`, il
programma principale ne attende la terminazione con `pthread_join()` e
termina a sua volta.

![image](/images/ambiente_globale/produttore_consumatore/prelievi_multipli.png)
