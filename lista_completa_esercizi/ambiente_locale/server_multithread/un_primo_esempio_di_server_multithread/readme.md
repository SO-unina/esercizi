# Un primo esempio di server multithread

Si realizzi in linguaggio C/C++ un processo servente **multithread**
basata su **code di messaggi UNIX**. Il processo servente, denominato
**Server**, attende di ricevere messaggi provenienti da un gruppo di 3
processi denominati **Client**. Ogni client invia 5 richieste di
elaborazione. Ogni messaggio inviato dovrà contenere il PID del Client e
due valori interi, selezionati casualmente tra 0 e 100 con la funzione
`rand()`<sup>[\[1\]](#footnote1)</sup>. I processi Client devono inoltre attendere e stampare a
video il messaggio di risposta del Server proveniente da un'apposita
coda dei messaggi di risposta, prima di mandare la richiesta successiva.
I processi Client devono essere generati dallo stesso programma
principale attraverso la primitiva `fork()`. Quando tutti i Client
terminano, il processo padre invia un messaggio speciale al Server,
contenenti la coppia di valori {-1, -1}, che causa la terminazione del
Server.

Il Server elabora i messaggi ricevuti come segue: estrae dal messaggio
la coppia di valori e il PID del Client, crea un nuovo thread tramite la
funzione `pthread_create()` a cui passa la coppia di valori e il PID, e
torna a ricevere il messaggio successivo. Ogni thread calcola il
prodotto della coppia di valori in ingresso, ed ha il compito di inviare
al Client (identificato dal PID) un messaggio contenente il valore
calcolato, tramite la coda di messaggi di risposta (da gestire in
**mutua esclusione**); dopodiché il thread termina. Quando il Server
riceve il messaggio con la coppia di valori {-1,-1}, termina.

![image](/images/ambiente_locale/server_multithread/un_primo_esempio_di_server_multithread.png)

### Soluzione 

``` {caption="Pseudo-codice del programma principale"}
creazione della coda dei messaggi di richiesta
creazione della coda dei messaggi di risposta

fork di un processo Server

for(1..3)
    fork di un processo Client

for(1..3)
    wait

invia messaggio {-1,-1} sulla coda delle richieste

wait
```

``` {caption="Pseudo-codice del programma Client"}

alloca struct messaggio richiesta (var. locale)
alloca struct messaggio risposta (var. locale)

inserisci PID nel messaggio richiesta

for(1..5)
    seleziona due interi casuali
    inserisci gli interi nel messaggio richiesta
    invia messaggio richiesta
    ricezione (bloccante) messaggio risposta, specificando il PID nel parametro "tipo"
    
```

``` {caption="Pseudo-codice del programma Server, thread principale"}

alloca un mutex pthread (var. globale)

alloca struct messaggio richiesta (var. locale)

while true

    ricezione (bloccante) messaggio richiesta, usando la struct come var. locale
    
    se il messaggio contiene {-1,-1}, terminare il processo
    
    alloca struct messaggio richiesta (var. dinamica)
    
    copia il messaggio richiesta dalla var. locale alla var. dinamica
    
    crea un thread "worker", passando la var. dinamica come parametro
    
```

``` {caption="Pseudo-codice del programma Server, thread worker"}

leggi i due valori interi dalla struct in ingresso
leggi il PID dalla struct in ingresso

alloca struct messaggio risposta (var. locale)

calcola il prodotto
scrivi il prodotto nel messaggio risposta
scrive il PID nel messaggio risposta

acquisisci il mutex globale

invia messaggio risposta

rilascia il mutex globale
```

<a name="footnote1">[1]</a>: La generazione casuale può essere implementata con la funzione
    rand() di stdlib.h; ad esempio: int incr = rand() % 101 rand()
    richiede che venga generato un seme dei numeri casuali attraverso la
    funzione srand(time(NULL)).
