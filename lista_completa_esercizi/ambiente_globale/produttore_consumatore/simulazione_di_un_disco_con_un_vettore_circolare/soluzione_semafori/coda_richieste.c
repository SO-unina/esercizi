#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>

#include "coda_richieste.h"

coda_richieste *inizializza_coda()
{
    int shm_id;
    int sem_id;
    coda_richieste *c;

    sem_id = semget(IPC_PRIVATE, 4, IPC_CREAT | 0644);

    if (sem_id < 0)
    {
        perror("Errore creazione semafori");
        exit(1);
    }

    semctl(sem_id, SPAZIO_DISP, SETVAL, DIM);
    semctl(sem_id, MESSAGGIO_DISP, SETVAL, 0);
    semctl(sem_id, MUTEX_P, SETVAL, 1);
    semctl(sem_id, MUTEX_C, SETVAL, 1);

    shm_id = shmget(IPC_PRIVATE, sizeof(coda_richieste), IPC_CREAT | 0644);

    if (shm_id < 0)
    {
        perror("Errore creazione SHM coda richieste");
        exit(1);
    }

    c = shmat(shm_id, NULL, 0);

    if (c == (void *)-1)
    {
        perror("Errore attach SHM");
        exit(1);
    }

    c->testa = 0;
    c->coda = 0;
    c->shm_id = shm_id;
    c->sem_id = sem_id;

    return c;
}

void preleva_richiesta(coda_richieste *c, richiesta *r)
{
    Wait_Sem(c->sem_id, MESSAGGIO_DISP);

    Wait_Sem(c->sem_id, MUTEX_C); /* Non necessario quando c'è un solo consumatore */

    printf("[%d] Consumazione in coda: %d\n", getpid(), c->coda);

    r->posizione = c->vettore[c->coda].posizione;
    r->processo = c->vettore[c->coda].processo;

    c->coda = (c->coda + 1) % DIM;

    Signal_Sem(c->sem_id, MUTEX_C); /* Non necessario quando c'è un solo consumatore */

    Signal_Sem(c->sem_id, SPAZIO_DISP);
}

void inserisci_richiesta(coda_richieste *c, richiesta *r)
{
    Wait_Sem(c->sem_id, SPAZIO_DISP);

    Wait_Sem(c->sem_id, MUTEX_P);

    printf("[%d] Produzione in testa: %d\n", getpid(), c->testa);

    c->vettore[c->testa].posizione = r->posizione;
    c->vettore[c->testa].processo = r->processo;

    c->testa = (c->testa + 1) % DIM;

    Signal_Sem(c->sem_id, MUTEX_P);

    Signal_Sem(c->sem_id, MESSAGGIO_DISP);
}

void rimuovi_coda(coda_richieste *c)
{
    semctl(c->sem_id, 0, IPC_RMID);
    shmctl(c->shm_id, IPC_RMID, NULL);
}
