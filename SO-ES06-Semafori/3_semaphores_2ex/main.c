#include "processi.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    /* Mapping anonimo condiviso: restando nella gerarchia padre-figlio
     * la memoria non ha bisogno di un nome. */
    shared_t *shared = mmap(NULL, sizeof(shared_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
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
    return result;
}
