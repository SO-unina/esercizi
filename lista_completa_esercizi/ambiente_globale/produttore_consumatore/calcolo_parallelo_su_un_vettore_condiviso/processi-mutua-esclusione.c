#include "processi.h"

#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MUTEX 0

int inizializza_semafori(condiviso_t *c)
{
    /* Valore iniziale: 1 (mutua esclusione).
     * pshared=1: il semaforo e' condiviso tra processi, purche'
     * collocato in una zona di memoria condivisa. */

    return sem_init(&c->semafori[MUTEX], 1, 1);
}

void distruggi_semafori(condiviso_t *c)
{
    sem_destroy(&c->semafori[MUTEX]);
}

void figlio(condiviso_t *c, int elemento_iniziale, int qta_elementi)
{

    printf("[FIGLIO] Ricerca del minimo: elementi da %d a %d\n", elemento_iniziale, elemento_iniziale + qta_elementi - 1);

    int minimo = INT_MAX;

    for (int i = elemento_iniziale; i < elemento_iniziale + qta_elementi; i++)
    {

        if (c->vettore[i] < minimo)
        {

            minimo = c->vettore[i];
        }
    }

    printf("[FIGLIO] Il minimo locale è %d\n", minimo);

    sem_wait(&c->semafori[MUTEX]);

    if (minimo < c->buffer)
    {

        c->buffer = minimo;
    }

    sem_post(&c->semafori[MUTEX]);
}

void padre(condiviso_t *c)
{

    /* Attesa terminazione processi figli */

    for (int i = 0; i < NUM_PROCESSI; i++)
    {

        wait(NULL);
    }

    /* Risultato finale */

    printf("[PADRE] Il valore minimo assoluto è: %d\n", c->buffer);
}
