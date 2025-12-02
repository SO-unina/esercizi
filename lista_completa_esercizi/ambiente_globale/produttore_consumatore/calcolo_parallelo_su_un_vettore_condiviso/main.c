#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>

#include "processi.h"

int main()
{

    int vett_id;
    int buffer_id;
    int sem_id;
    int *vettore;
    int *buffer;

    vett_id = shmget(IPC_PRIVATE, sizeof(int) * NUM_ELEMENTI, IPC_CREAT | 0644);

    if (vett_id < 0)
    {
        perror("Impossibile creare l'array di interi condiviso");
        exit(1);
    }

    vettore = shmat(vett_id, NULL, 0);

    if (vettore == (void *)-1)
    {
        perror("Impossibile inizializzare l'array di interi condiviso");
        exit(1);
    }

    /* Inizializza il vettore con numeri casuali tra 0 e INT_MAX */

    srand(time(NULL));

    for (int i = 0; i < NUM_ELEMENTI; i++)
    {

        vettore[i] = rand() % INT_MAX;

        //printf("%d\n", vettore[i]); // per debugging
    }

    buffer_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0644);

    if (buffer_id < 0)
    {
        perror("Impossibile creare il buffer condiviso");
        exit(1);
    }

    buffer = shmat(buffer_id, NULL, 0);

    if (buffer == (void *)-1)
    {
        perror("Impossibile inizializzare il buffer condiviso");
        exit(1);
    }

    /* Inizializza il buffer ad INT_MAX.
     * Il valore da ricercare sarà, per definizione, minore del valore iniziale.
     */

    *buffer = INT_MAX;


    /* Inizializzazione semafori */

    sem_id = inizializza_semafori();



    /* Avvio dei processi figli */

    pid_t pid;

    for (int i = 0; i < 10; i++)
    {

        pid = fork();

        if (pid == 0)
        {
            /* Processo figlio */

            /* NUM_ELEMENTI/NUM_PROCESSI = 1000
             * i=0: 0...999
             * i=1: 1000...1999
             * i=2: 2000...2999
             * ...
             * i=9: 9000...9999
             */
            figlio(vettore,
                    buffer,
                    sem_id,
                    i * (NUM_ELEMENTI / NUM_PROCESSI),
                    NUM_ELEMENTI / NUM_PROCESSI);

            exit(0);
        }

        else if (pid < 0)
        {
            perror("Impossibile avviare processo figlio");
            exit(1);
        }
    }


    /* Processo padre */

    padre(buffer, sem_id);


    /* Deallocazione risorse IPC */

    semctl(sem_id, 0, IPC_RMID);
    shmctl(vett_id, IPC_RMID, 0);
    shmctl(buffer_id, IPC_RMID, 0);
}
