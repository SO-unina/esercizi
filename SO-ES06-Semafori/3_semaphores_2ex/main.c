#include "processi.h"

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    char name[64];
    sprintf(name, "/so_es06_min_%d", getpid());

    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(fd, sizeof(shared_t)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    shared_t *shared = mmap(NULL, sizeof(shared_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    if (sem_init(&shared->mutex, 1, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
    shared->minimo = INT_MAX;

    srand(1);
    for (int i = 0; i < VECTOR_SIZE; i++) {
        shared->vettore[i] = rand() % 100000;
    }

    /* Calcolo sequenziale usato per verificare il risultato parallelo. */
    int controllo = shared->vettore[0];
    for (int i = 1; i < VECTOR_SIZE; i++) {
        if (shared->vettore[i] < controllo) {
            controllo = shared->vettore[i];
        }
    }

    for (int i = 0; i < CHILDREN; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            ricerca_minimo(shared, i);
        }
    }

    for (int i = 0; i < CHILDREN; i++) {
        if (wait(NULL) == -1) {
            perror("wait");
            exit(1);
        }
    }

    printf("Minimo parallelo: %d; controllo sequenziale: %d\n", shared->minimo, controllo);

    int result = 1;
    if (shared->minimo == controllo) {
        result = 0;
    }

    sem_destroy(&shared->mutex);
    munmap(shared, sizeof(shared_t));
    close(fd);
    shm_unlink(name);
    return result;
}
