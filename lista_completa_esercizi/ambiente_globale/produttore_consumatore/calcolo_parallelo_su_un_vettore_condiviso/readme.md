# Calcolo parallelo su un vettore condiviso

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso** che
svolga, in parallelo su più processi, la **ricerca del valore minimo**
all'interno di un vettore di grandi dimensioni. Il processo padre dovrà
allocare un vettore di 10.000 valori interi, da memorizzare in una
**memoria condivisa UNIX**, e da inizializzare con valori non negativi
scelti casualmente. Dopodiché, il processo padre dovrà creare un gruppo
di 10 processi figli, e scambiare con loro il vettore di interi.

Ciascuno dei processi figli dovrà lavorare su **una porzione distinta
del vettore** creato dal padre. Ad esempio, il primo figlio dovrà
lavorare sui primi 1000 elementi; il secondo figlio sui secondi 1000
elementi; etc. Ogni figlio dovrà cercare il valore minimo all'interno
dalla porzione di vettore assegnatagli.

**Prima versione dell'esercizio.** Il processo padre e i figli dovranno
inoltre condividere in **mutua esclusione** un buffer, atto a contenere
un singolo valore intero, utilizzando **una area di memoria condivisa e
semafori UNIX**. Al termine della ricerca, il processo figlio dovrà
consultare il valore attualmente presente nel buffer condiviso. Se il
valore che ha trovato è minore di quello del buffer, il processo figlio
dovrà sovrascrivere il buffer con il valore che ha trovato. Al termine
di tutti i processi figli, il padre potrà prelevare il valore minimo
assoluto dal buffer condiviso.

**Seconda versione dell'esercizio.** Si svolga inoltre una versione
alternativa di questo esercizio, utilizzando lo schema
**produttore-consumatore con buffer singolo**, utilizzando anche questa
volta **una area di memoria condivisa e semafori UNIX**. Ogni processo
figlio, al termine della propria ricerca, dovrà effettuare una
produzione sul buffer condiviso, utilizzando il valore trovato, e
sospendendosi in attesa se il buffer dovesse essere già occupato. Il
processo padre dovrà effettuare tante consumazioni quanti sono i
processi figli, per raccogliere i valori minimi trovati da ogni figlio,
e conservare il valore minimo tra tutti i valori raccolti.

![image](/images/ambiente_globale/produttore_consumatore/calcolo_parallelo_su_un_vettore_condiviso.png)
