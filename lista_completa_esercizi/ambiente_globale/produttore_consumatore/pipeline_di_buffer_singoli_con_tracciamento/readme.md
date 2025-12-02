# Pipeline di buffer singoli, con tracciamento

Si realizzi un'applicazione **multithread** di tipo
**Produttore-Consumatore**, utilizzando la libreria PThreads. Oltre ad
avere 1 thread Produttore ed 1 thread Consumatore, il programma dovrà
avere due ulteriori thread **Intermediario** e **Misuratore**, come in
figura.

Il programma dovrà allocare **due buffer singoli** (di tipo intero) in
memoria dinamica (heap); inoltre, dovrà allocare una **ulteriore area
dinamica ("statistiche")** atta a contenere una coppia di interi, che
rappresentino rispettivamente il numero di messaggi e la somma dei
valori dei messaggi. L'accesso alla struttura dati "statistiche" dovrà
avvenire in **mutua esclusione** mediante uno specifico mutex.

Il thread Produttore dovrà generare valori interi casuali tra 0 e 9, e
collocarli su un primo buffer singolo. Il thread Intermediario dovrà
fungere da consumatore sul primo buffer. Ad ogni consumazione,
l'Intermediario dovrà aggiornare la struttura dati "statistiche",
incrementando di 1 il valore intero con il conteggio dei messaggi, e
sommando il valore del messaggio consumato alla somma totale dei valori
dei messaggi. Infine, il thread Intermediario dovrà collocare il valore
consumato nel secondo buffer singolo, e il thread Consumatore dovrà
prelevarne i messaggi.

Il thread Misuratore dovrà periodicamente (ogni secondo) leggere la
struttura "statistiche", e stampare a video il numero totale di
messaggi, e calcolare e stampare il valore medio dei messaggi (ossia, il
rapporto tra la somma dei valori trasmessi e il numero totale dei
messaggi). Dopo la stampa, il thread dovrà porre a zero i due campi
interi della struttura "statistiche".

I thread Produttore, Intermediario e Consumatore dovranno lavorare senza
attese tra le operazioni, e fare in totale rispettivamente 500.000
produzioni e consumazioni (o altra quantità atta a far durare
l'esecuzione per alcuni secondi), per poi terminare. Ad ogni stampa, si
prefigga una stringa identificativa del thread (es. "\[PRODUTTORE\]",
"\[INTERMEDIARIO\]", etc.).

![image](/images/ambiente_globale/produttore_consumatore/pipeline_di_buffer_singoli_con_tracciamento.png)
