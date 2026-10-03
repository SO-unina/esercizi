#include "header.h"

#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void) {
    /* Apre l'oggetto di shared memory gia' creato dal programma
     * "clienti": senza O_CREAT, se l'oggetto non esiste si ha errore. */
    int fd = shm_open(TEATRO_SHM_NAME, O_RDWR, 0);
    if (fd == -1) {
        perror("shm_open (avviare prima ./clienti)");
        exit(1);
    }

    teatro_t *teatro = mmap(NULL, sizeof(teatro_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (teatro == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);

    while (1) {
        sem_wait(&teatro->mutex);

        for (int i = 0; i < POSTI; i++) {
            char stato = '\0';

            if (teatro->posti[i].stato == LIBERO) {
                stato = 'L';
            } else if (teatro->posti[i].stato == INAGGIORNAMENTO) {
                stato = 'A';
            } else if (teatro->posti[i].stato == OCCUPATO) {
                stato = 'O';
            }

            printf("%d:%c ", i, stato);
        }

        printf("\n\n");

        sem_post(&teatro->mutex);

        sleep(1);
    }

    return 0;
}
