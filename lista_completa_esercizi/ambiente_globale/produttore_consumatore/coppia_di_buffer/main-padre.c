#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "buffer.h"

int main()
{

    int buf1_id;
    int buf2_id;
    buffer *buf1;
    buffer *buf2;
    int sem_id;

    buf1_id = shmget(ftok(".", 'a'), sizeof(buffer), IPC_CREAT | 0644);

    if (buf1_id < 0)
    {
        perror("Errore creazione SHM buf1");
        exit(1);
    }

    buf2_id = shmget(ftok(".", 'b'), sizeof(buffer), IPC_CREAT | 0644);

    if (buf2_id < 0)
    {
        perror("Errore creazione SHM buf2");
        exit(1);
    }

    sem_id = semget(ftok(".", 'c'), 2, IPC_CREAT | 0644);

    if (sem_id < 0)
    {
        perror("Errore creazione semafori");
        exit(1);
    }

    buf1 = shmat(buf1_id, NULL, 0);

    if (buf1 == (void *)-1)
    {
        perror("Errore attach SHM buf1");
        exit(1);
    }

    buf2 = shmat(buf2_id, NULL, 0);

    if (buf2 == (void *)-1)
    {
        perror("Errore attach SHM buf2");
        exit(1);
    }

    buf1->stato = LIBERO;
    buf2->stato = LIBERO;

    semctl(sem_id, SPAZIO_DISP, SETVAL, 2);
    semctl(sem_id, MESSAGGIO_DISP, SETVAL, 0);

    pid_t pid1 = fork();

    if (pid1 < 0)
    {
        perror("Errore creazione produttore");
        exit(1);
    }
    else if (pid1 == 0)
    {
        /* figlio */

        execl("./main-produttore", "main-produttore", NULL);
    }

    pid_t pid2 = fork();

    if (pid2 < 0)
    {
        perror("Errore creazione consumatore");
        exit(1);
    }
    else if (pid2 == 0)
    {
        /* figlio */

        execl("./main-consumatore", "main-consumatore", NULL);
    }

    for (int i = 0; i < 2; i++)
    {
        wait(NULL);
    }

    shmctl(buf1_id, IPC_RMID, NULL);
    shmctl(buf2_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
}
