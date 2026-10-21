#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
    sem_t sem;
    int value;
} shared_t;

int main(void) {
    /* Mapping ANONIMO condiviso: restando nella gerarchia padre-figlio
     * la memoria non ha bisogno di un nome. Niente shm_open, ftruncate
     * ne' unlink: il figlio eredita il mapping con la fork e la memoria
     * sparisce da sola quando terminano tutti i processi. */
    shared_t *shared = mmap(NULL, sizeof(shared_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    /* pshared=1 rende il semaforo utilizzabile da processi differenti. */
    if (sem_init(&shared->sem, 1, 0) == -1) {
        perror("sem_init");
        return 1;
    }
    shared->value = 0;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        if (sem_wait(&shared->sem) == -1) {
            perror("sem_wait");
            exit(1);
        }

        printf("Figlio: value=%d\n", shared->value);
        exit(0);
    }

    /* Il padre pubblica prima il dato e poi risveglia il figlio. */
    shared->value = 42;
    if (sem_post(&shared->sem) == -1) {
        perror("sem_post");
        return 1;
    }

    if (wait(NULL) == -1) {
        perror("wait");
        return 1;
    }

    sem_destroy(&shared->sem);
    munmap(shared, sizeof(shared_t));
    return 0;
}
