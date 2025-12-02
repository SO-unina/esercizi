# Media e deviazione standard di un vettore

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso** che
effettui il calcolo di media e deviazione standard sugli elementi di un
vettore di buffer. La sincronizzazione deve essere gestita mediante
**semafori**.


    #define N 5

    typedef struct{
      int elementi[N];
      int testa;
      int coda;
    } BufferCircolare;

    void produci_elemento(int sem_id, BufferCircolare* buf);
    void consuma_elementi(int sem_id, BufferCircolare* buf);

Il vettore di buffer condiviso tra produttori e consumatori è una **coda
circolare** su una **shared memory UNIX**. La funzione
`produci_elemento`, richiamata dal processo produttore, genera
**esattamente un elemento** casuale tra 1 e 10 e lo posiziona
all'interno della coda circolare. Il produttore si sospende in attesa su
un semaforo nel caso in cui trovi il buffer pieno.

Il processo consumatore, invece, richiama la funzione
`consuma_elementi`. È richiesto che il consumatore si blocchi nel caso
in cui **il vettore di buffer non sia pieno**. In altri termini, il
consumatore si sospende se il vettore contiene meno di N elementi, e si
sblocca **se e solo se il vettore di buffer è pieno**. Una volta
sbloccato, il consumatore dovrà **consumare tutti gli elementi del
vettore**, e calcolare media e deviazione standard degli elementi,
stampando a video i risultati.

::: center
![image](/images/ambiente_globale/produttore_consumatore/media_e_deviazione_standard_di_un_vettore.png)
:::

I processi produttore e consumatore dovranno essere istanziati
utilizzando **due eseguibili separati**. Un terzo programma **master**
dovrà istanziare 10 processi produttore e 1 processo consumatore, e
attenderne la terminazione. Tra la creazione di un processo e la
successiva si dovrà attendere un tempo casuale tra 1 e 3 secondi. Ogni
produttore dovrà effettuare una sola produzione. Il processo consumatore
effettuerà 2 accessi al vettore, attendendo 1 secondo tra i 2 accessi.
