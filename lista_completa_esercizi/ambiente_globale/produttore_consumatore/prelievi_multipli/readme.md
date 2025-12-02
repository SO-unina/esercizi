# Prelievi multipli

Si realizzi in linguaggio C/C++ un'applicazione **multi-processo** in
cui P produttori e C consumatori scambiano dati attraverso una buffer
circolare (politica FIFO) di N elementi di tipo intero, allocato in una
memoria condivisa.

I processi produttori produrranno **un solo elemento ad ogni
produzione**, mentre i processi consumatori consumeranno **due elementi
ad ogni consumazione**. Un produttore deve bloccarsi se il buffer a cui
tenta di accedere è pieno, finché non c'è spazio disponibile. I
consumatori possono prelevare dal buffer se ci sono elementi
disponibili; se il buffer è vuoto, i consumatori devono bloccarsi fino a
quando non ci sono nuovi elementi nel buffer. L'accesso al buffer e ai
relativi puntatori di testa e coda deve essere disciplinato attraverso
il costrutto Monitor di Hoare.

Il programma dovrà istanziare 5 processi produttori, ciascuno dei quali
produrrà un elemento per 6 volte, attendendo due secondi tra una
produzione e l'altra. Inoltre, si dovranno istanziare 5 processi
consumatori, ciascuno dei quali preleverà due elementi dal buffer per 3
volte, attendendo un secondo tra una consumazione e l'altra; gli
elementi prelevati saranno stampati a video. Una volta istanziati i
processi, tramite la primitiva `fork()`, il programma principale ne
attende la terminazione e termina a sua volta.

![image](/images/ambiente_globale/produttore_consumatore/prelievi_multipli.png)
