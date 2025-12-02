#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "prodcons.h"

#define DIM 4

int main()
{

    int vettore_id;
    int stato_id;
    int sem_id;
    buffer *vettore;
    int *stato;

    vettore_id = shmget(IPC_PRIVATE, sizeof(buffer) * DIM, IPC_CREAT | 0644);

    if (vettore_id < 0)
    {
        perror("Errore creazione SHM vettore di buffer");
        exit(1);
    }

    stato_id = shmget(IPC_PRIVATE, sizeof(buffer) * DIM, IPC_CREAT | 0644);

    if (stato_id < 0)
    {
        perror("Errore creazione SHM vettore di stato");
        exit(1);
    }

    sem_id = semget(IPC_PRIVATE, 5, IPC_CREAT | 0644);

    if (sem_id < 0)
    {
        perror("Errore creazione semafori");
        exit(1);
    }

    vettore = shmat(vettore_id, NULL, 0);

    if (vettore == (void *)-1)
    {
        perror("Errore attach SHM vettore di buffer");
        exit(1);
    }

    stato = shmat(stato_id, NULL, 0);

    if (stato == (void *)-1)
    {
        perror("Errore attach SHM vettore di stato");
        exit(1);
    }

    for(int i=0; i<DIM; i++) {
        stato[i] = LIBERO;
    }

    semctl(sem_id, SPAZIO_DISP, SETVAL, DIM);
    semctl(sem_id, MESSAGGIO_DISP_1, SETVAL, 0);
    semctl(sem_id, MESSAGGIO_DISP_2, SETVAL, 0);
    semctl(sem_id, MESSAGGIO_DISP_3, SETVAL, 0);
    semctl(sem_id, MUTEX_P, SETVAL, 1);

    for (int i = 0; i < 3; i++)
    {

        pid_t pid_produttore = fork();

        if (pid_produttore == 0)
        {
            /* figlio */

            printf("[%d] Avvio produttore\n", getpid());

            srand(getpid());

            int chiave = i;

            for(int j=0; j<3; j++) {

                int valore = rand() % 10;
                produzione(sem_id, vettore, stato, DIM, chiave, valore);
            }

            exit(0);
        }
        else if (pid_produttore < 0)
        {
            perror("Errore fork produttore");
            exit(1);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        pid_t pid_consumatore = fork();

        if (pid_consumatore == 0)
        {
            /* figlio */

            printf("[%d] Avvio consumatore\n", getpid());

            int chiave = i;

            for(int j=0; j<3; j++) {

                int valore = consumazione(sem_id, vettore, stato, DIM, chiave);
            }

            exit(0);
        }
        else if (pid_consumatore < 0)
        {
            perror("Errore fork consumatore");
            exit(1);
        }
    }

    for (int i = 0; i < 6; i++)
    {
        wait(NULL);
    }

    shmctl(vettore_id, IPC_RMID, NULL);
    shmctl(stato_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
}