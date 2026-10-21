#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "condiviso.h"

/* Lo scrittore deposita il dato nella shm e poi SEGNALA con il semaforo
 * nominato: cooperazione, non mutua esclusione. */

int main(void) {
    int fd = shm_open(SHM_NAME, O_RDWR, 0);
    if (fd == -1) {
        perror("shm_open (avviare tramite ./master)");
        exit(1);
    }

    condiviso_t *c = mmap(NULL, sizeof(condiviso_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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

    printf("[scrittore %d] Preparo il dato...\n", getpid());
    sleep(2);

    c->valore = 42;
    printf("[scrittore %d] Dato pronto, segnalo al lettore\n", getpid());

    sem_post(pronto);

    sem_close(pronto);
    munmap(c, sizeof(condiviso_t));
    exit(0);
}
