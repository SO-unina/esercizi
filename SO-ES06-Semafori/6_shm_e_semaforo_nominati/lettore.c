#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "condiviso.h"

/* Il lettore attende la segnalazione sul semaforo nominato e solo
 * allora legge il dato dalla shm. */

int main(void) {
    int fd = shm_open(SHM_NAME, O_RDONLY, 0);
    if (fd == -1) {
        perror("shm_open (avviare tramite ./master)");
        exit(1);
    }

    condiviso_t *c = mmap(NULL, sizeof(condiviso_t), PROT_READ, MAP_SHARED, fd, 0);
    if (c == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);

    sem_t *pronto = sem_open(SEM_PRONTO, 0);
    if (pronto == SEM_FAILED) {
        perror("sem_open (avviare tramite ./master)");
        exit(1);
    }

    printf("[lettore %d] Attendo il dato...\n", getpid());

    sem_wait(pronto);

    printf("[lettore %d] Dato ricevuto: valore=%d\n", getpid(), c->valore);

    sem_close(pronto);
    munmap(c, sizeof(condiviso_t));
    exit(0);
}
