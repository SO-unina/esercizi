#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "condiviso.h"

/* Dopo la exec il lavoratore non ha alcun mapping ereditato: ritrova la
 * risorsa PER NOME. Il semaforo arriva "gratis" dentro la struct. */

int main(void) {
    /* Senza O_CREAT: l'oggetto deve gia' esistere (lo crea il master). */
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

    for (int k = 0; k < INCREMENTI; k++) {
        sem_wait(&c->mutex);

        int temp = c->contatore;
        sleep(1);
        c->contatore = temp + 1;
        printf("[lavoratore %d] contatore: %d\n", getpid(), c->contatore);

        sem_post(&c->mutex);
    }

    /* Chi usa la risorsa rilascia solo il proprio mapping. */
    munmap(c, sizeof(condiviso_t));
    exit(0);
}
