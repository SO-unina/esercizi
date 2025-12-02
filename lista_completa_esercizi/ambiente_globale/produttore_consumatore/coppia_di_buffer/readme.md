# Coppia di buffer

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso** in cui
un processo **produttore** ed un processo **consumatore** scambiano dati
attraverso due buffer condivisi, allocati ognuno in una **shared memory
UNIX**. Un buffer dovrà contenere un intero tra 0 e 9. Inoltre, ognuno
dei due buffer dovrà contenere una variabile di stato, che denota se il
suo stato è \"libero\" (il produttore può depositarvi un dato),
\"occupato\" (il consumatore può prelevare un dato), oppure \"in uso\"
(un produttore oppure un consumatore sta correntemente lavorando sul
buffer, per cui non è ammesso ad un altro processo di accedervi). Si
sincronizzi l'accesso ai due buffer da parte dei processi facendo in
modo che il produttore si sospenda se entrambi i buffer sono occupati
e/o in uso, e il consumatore si sospenda se entrambi i buffer sono
liberi e/o in uso. Si sincronizzi l'accesso utilizzando **semafori
UNIX**. Si simuli la produzione con una attesa di 1 secondo, e la
consumazione con una attesa di 2 secondi, effettuando in entrambi i casi
una stampa a video. In totale, il produttore dovrà effettuare 5
produzioni, e il consumatore 5 consumazioni.

Il programma deve essere sviluppato in tre eseguibili distinti, di cui:
(1) il primo eseguibile è eseguito da un processo padre che crea due
processi figli, ciascuno dei quali eseguirà ognuno degli altri due
eseguibili; (2) un eseguibile per il codice del consumatore; (3) un
eseguibile per il codice del produttore.

![image](/images/ambiente_globale/produttore_consumatore/coppia_di_buffer.png)
