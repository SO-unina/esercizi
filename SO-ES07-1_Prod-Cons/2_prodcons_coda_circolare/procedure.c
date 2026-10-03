#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"


void produttore(struct prodcons * p) {

    //printf("produttore è fermo prima di wait\n");
    sem_wait(&p->spazio_disponibile);
    //printf("produttore si sblocca dopo la wait\n");


    sem_wait(&p->mutex_p);


    sleep(2);

    // genera valore tra 0 e 99
    p->buffer[p->testa] = rand() % 100;

    printf("Il valore prodotto = %d\n", p->buffer[p->testa]);

    p->testa = (p->testa+1) % DIM_BUFFER;


    sem_post(&p->mutex_p);

    sem_post(&p->messaggio_disponibile);
}

void consumatore(struct prodcons * p) {

    //printf("consumatore è fermo prima di wait\n");
    sem_wait(&p->messaggio_disponibile);
    //printf("consumatore si sblocca dopo la wait\n");

    sem_wait(&p->mutex_c);


    sleep(2);

    printf("Il valore consumato = %d\n", p->buffer[p->coda]);

    p->coda = (p->coda + 1) % DIM_BUFFER;


    sem_post(&p->mutex_c);

    sem_post(&p->spazio_disponibile);
}
