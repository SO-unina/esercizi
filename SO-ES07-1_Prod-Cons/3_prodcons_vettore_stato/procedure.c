#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"


void produttore(struct prodcons * p) {

    int indice = 0;


    sem_wait(&p->spazio_disponibile);


    sem_wait(&p->mutex_p);

    while(indice < DIM_BUFFER && p->stato[indice] != BUFFER_VUOTO) {
        /*
            indice < DIM_BUFFER non dovrebbe mai capitare, ma è importante evitare SEMPRE di leggere fuori dal buffer (crash o comportamento indefinito) e
            con la guardia usciamo dal ciclo senza danni.
        */
        indice++;
    }

    p->stato[indice] = BUFFER_INUSO;

    //qui devo rilasciare il mutex per i produttori...ERRORE se non lo faccio!!!!
    sem_post(&p->mutex_p);


    sleep(2);

    // genera valore tra 0 e 99
    p->buffer[indice] = rand() % 100;

    printf("Il valore prodotto = %d\n", p->buffer[indice]);


    p->stato[indice] = BUFFER_PIENO;

    sem_post(&p->messaggio_disponibile);
}

void consumatore(struct prodcons * p) {

    int indice = 0;


    sem_wait(&p->messaggio_disponibile);


    sem_wait(&p->mutex_c);

    while(indice < DIM_BUFFER && p->stato[indice] != BUFFER_PIENO) {
        /*
            indice < DIM_BUFFER non dovrebbe mai capitare, ma è importante evitare SEMPRE di leggere fuori dal buffer (crash o comportamento indefinito) e
            con la guardia usciamo dal ciclo senza danni.
        */
        indice++;
    }

    p->stato[indice] = BUFFER_INUSO;

    //qui devo rilasciare il mutex per i consumatori...ERRORE se non lo faccio!!!!
    sem_post(&p->mutex_c);


    sleep(2);

    printf("Il valore consumato = %d\n", p->buffer[indice]);


    p->stato[indice] = BUFFER_VUOTO;

    sem_post(&p->spazio_disponibile);
}
