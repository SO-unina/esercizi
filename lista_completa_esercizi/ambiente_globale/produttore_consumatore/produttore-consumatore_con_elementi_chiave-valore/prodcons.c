#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "prodcons.h"

#include <semaphore.h>

void produzione(prodcons_t *pc, int chiave, int valore)
{

    printf("[%d] Produttore, chiave %d, accede...\n", getpid(), chiave);

    sem_wait(&pc->spazio_disp);

    sem_wait(&pc->mutex_p);

    int i = 0;
    while (pc->stato[i] != LIBERO)
    {
        i++;
    }

    pc->stato[i] = INUSO;

    printf("[%d] Produttore, chiave %d, buffer %d in uso...\n", getpid(), chiave, i);

    sem_post(&pc->mutex_p);

    sleep(rand() % 3 + 1);

    printf("[%d] Produttore, chiave %d, buffer %d, valore=%d\n", getpid(), chiave, i, valore);

    pc->vettore[i].chiave = chiave;
    pc->vettore[i].valore = valore;

    pc->stato[i] = OCCUPATO;

    /* Un semaforo "messaggio disponibile" per ogni chiave */

    if (chiave == 1)
    {
        sem_post(&pc->messaggio_disp_1);
    }
    else if (chiave == 2)
    {
        sem_post(&pc->messaggio_disp_2);
    }
    else if (chiave == 3)
    {
        sem_post(&pc->messaggio_disp_3);
    }
}

int consumazione(prodcons_t *pc, int chiave)
{

    int valore;

    printf("[%d] Consumatore, chiave %d, accede...\n", getpid(), chiave);

    /* Il consumatore attende un messaggio con la propria chiave */

    if (chiave == 1)
    {
        sem_wait(&pc->messaggio_disp_1);
    }
    else if (chiave == 2)
    {
        sem_wait(&pc->messaggio_disp_2);
    }
    else if (chiave == 3)
    {
        sem_wait(&pc->messaggio_disp_3);
    }

    int i = 0;
    while (pc->stato[i] != OCCUPATO || pc->vettore[i].chiave != chiave)
    {
        i++;
    }

    pc->stato[i] = INUSO;

    printf("[%d] Consumatore, chiave %d, buffer %d in uso...\n", getpid(), chiave, i);

    sleep(rand() % 3 + 1);

    valore = pc->vettore[i].valore;

    printf("[%d] Consumatore, chiave %d, buffer %d, valore=%d\n", getpid(), chiave, i, valore);

    pc->stato[i] = LIBERO;

    sem_post(&pc->spazio_disp);

    return valore;
}
