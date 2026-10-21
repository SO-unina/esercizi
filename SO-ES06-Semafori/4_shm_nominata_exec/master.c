#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include "condiviso.h"

/* I quattro scenari d'uso di memoria condivisa e semafori:
 *
 * - Caso 1: solo fork. Memoria anonima (MAP_SHARED | MAP_ANONYMOUS) e
 *   semafori con sem_init dentro: nessun nome, i figli ereditano tutto.
 *
 * - Caso 2: eseguibili distinti con dati condivisi. Shm nominata
 *   (shm_open), semafori anonimi dentro la struct: chi arriva rifa'
 *   shm_open e mmap e la sem_open non serve. E' QUESTO esempio.
 *
 * - Caso 3: eseguibili distinti senza dati condivisi. Il nome sta sul
 *   semaforo: sem_open in tutti, sem_unlink nel responsabile.
 *
 * - Caso 4: eseguibili distinti, dati condivisi e semafori nominati per
 *   la cooperazione: la shm nominata contiene solo i dati, i semafori
 *   vivono fuori dalla struct come oggetti nominati a se'. */

int main(void) {
    /* Elimina un eventuale oggetto rimasto da una precedente esecuzione. */
    shm_unlink(SHM_NAME);

    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(fd, sizeof(condiviso_t)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    condiviso_t *c = mmap(NULL, sizeof(condiviso_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (c == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);

    sem_init(&c->mutex, 1, 1);
    c->contatore = 0;

    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            execl("./lavoratore", "./lavoratore", (char *)0);

            perror("Exec fallita");
            exit(1);
        }
    }

    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }

    printf("Valore finale: %d (atteso: %d)\n", c->contatore, 2 * INCREMENTI);

    /* Il creatore distrugge il semaforo e rimuove il nome. */
    sem_destroy(&c->mutex);
    munmap(c, sizeof(condiviso_t));
    shm_unlink(SHM_NAME);
    return 0;
}
