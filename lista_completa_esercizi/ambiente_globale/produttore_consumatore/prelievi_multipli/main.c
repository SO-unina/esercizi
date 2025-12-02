#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "prodcons.h"

int main()
{

    int shm_id = shmget(IPC_PRIVATE, sizeof(ProdCons), IPC_CREAT | 0644);

    if (shm_id < 0)
    {
        perror("Errore creazione shared memory");
        exit(1);
    }

    ProdCons *p = shmat(shm_id, NULL, 0);

    if (p == (void *)-1)
    {
        perror("Errore attach shared memory");
        exit(1);
    }

    inizializza(p);

    for (int i = 0; i < 5; i++)
    {

        pid_t pid = fork();

        if (pid == 0)
        {

            /* Produttore */

            printf("[%d] Avvio produttore\n", getpid());

            srand(getpid());

            for (int j = 0; j < 6; j++)
            {

                int val = rand() % 10;

                produci(p, val);

                sleep(2);
            }

            exit(0);
        }
    }

    for (int i = 0; i < 5; i++)
    {

        pid_t pid = fork();

        if (pid == 0)
        {

            /* Consumatore */

            printf("[%d] Avvio consumatore\n", getpid());

            for (int j = 0; j < 3; j++)
            {

                int val_1, val_2;

                consuma(p, &val_1, &val_2);

                sleep(1);
            }

            exit(0);
        }
    }

    printf("[%d] Processo padre in attesa...\n", getpid());

    for (int i = 0; i < 10; i++)
    {
        wait(NULL);
    }

    printf("[%d] Terminazione\n", getpid());

    rimuovi(p);

    shmctl(shm_id, IPC_RMID, 0);

    return 0;
}
