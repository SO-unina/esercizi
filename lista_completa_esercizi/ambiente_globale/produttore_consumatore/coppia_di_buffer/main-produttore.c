#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "buffer.h"

int main()
{

    int buf1_id;
    int buf2_id;
    buffer *buf1;
    buffer *buf2;
    int sem_id;

    buf1_id = shmget(ftok(".", 'a'), sizeof(buffer), 0);

    if (buf1_id < 0)
    {
        perror("Errore accesso SHM buf1");
        exit(1);
    }

    buf2_id = shmget(ftok(".", 'b'), sizeof(buffer), 0);

    if (buf2_id < 0)
    {
        perror("Errore accesso SHM buf2");
        exit(1);
    }

    sem_id = semget(ftok(".", 'c'), 2, 0);

    if (sem_id < 0)
    {
        perror("Errore accesso semafori");
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

    srand(time(NULL));

    for (int i = 0; i < 5; i++)
    {

        int valore = rand() % 10;

        produzione(sem_id, buf1, buf2, valore);
    }

    exit(0);
}
