#include "processi.h"

#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MESSAGGIO_DISP 0
#define SPAZIO_DISP 1

int inizializza_semafori(condiviso_t *c)
{
    /* Valori iniziali: 0 (messaggio disponibile), 1 (spazio disponibile).
     * pshared=1: i semafori sono condivisi tra processi, purche'
     * collocati in una zona di memoria condivisa. */

    if (sem_init(&c->semafori[MESSAGGIO_DISP], 1, 0) < 0)
    {
        return -1;
    }

    return sem_init(&c->semafori[SPAZIO_DISP], 1, 1);
}

void distruggi_semafori(condiviso_t *c)
{
    sem_destroy(&c->semafori[MESSAGGIO_DISP]);
    sem_destroy(&c->semafori[SPAZIO_DISP]);
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

    sem_wait(&c->semafori[SPAZIO_DISP]);

    c->buffer = minimo;

    sem_post(&c->semafori[MESSAGGIO_DISP]);
}

void padre(condiviso_t *c)
{

    int minimo = INT_MAX;

    for (int i = 0; i < NUM_PROCESSI; i++)
    {
        sem_wait(&c->semafori[MESSAGGIO_DISP]);

        if( c->buffer < minimo ) {

            minimo = c->buffer;
        }

        sem_post(&c->semafori[SPAZIO_DISP]);
    }

    /* Attesa terminazione processi figli */

    for (int i = 0; i < NUM_PROCESSI; i++)
    {

        wait(NULL);
    }

    /* Risultato finale */

    printf("[PADRE] Il valore minimo assoluto è: %d\n", minimo);
}
