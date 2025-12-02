# Produttore/consumatore con elementi chiave-valore

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso**, che
si basi su **semafori e memorie condivise UNIX**, e sullo schema
**produttore-consumatore con vettore di stato**. I processi dovranno
condividere un vettore di 4 buffer ed un vettore di stato, ed ogni
buffer dovrà contenere una coppia di interi, denominati **chiave** e
**valore**.

Il programma dovrà generare tre processi produttori, ciascuno dei quali
produrrà una coppia con un valore casuale e una chiave rispettivamente
pari a 1, 2, e 3. Inoltre, il programma dovrà generare tre processi
consumatori, ciascuno dei quali consumerà le coppie con chiave
rispettivamente pari a 1, 2, e 3. Ad esempio, il primo processo
produttore produrrà una coppia con chiave 1; il primo processo
consumatore consumerà una coppia con chiave 1 (ignorando le coppie con
chiave 2 oppure 3), oppure dovrà sospendersi in attesa nel caso che non
siano disponibili delle coppie con chiave 1. Si noti che sarà necessario
**utilizzare 3 semafori di tipo "messaggio disponibile", uno per ogni
chiave**.

Le operazioni di produzione e consumazione (durante cui un buffer è
nello stato "in uso") dovranno durare un tempo casuale tra 1 e 3
secondi, utilizzando la primitiva `sleep()`. In totale, i processi
dovranno produrre o consumare 3 coppie chiave-valore.

![image](/images/ambiente_globale/produttore_consumatore/produttore-consumatore_con_elementi_chiave-valore.png)
