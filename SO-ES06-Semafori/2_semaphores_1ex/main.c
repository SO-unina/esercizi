#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    sem_t mutex;
    int valore;
} shared_t;

static void figlio(shared_t *shared) {
    srand(time(NULL) + getpid());

    for (int i = 0; i < 10; i++) {
        printf("[%d] Attendo di entrare nella sezione critica\n", getpid());

        if (sem_wait(&shared->mutex) == -1) {
            exit(1);
        }

        printf("[%d] Sono entrato nella sezione critica\n", getpid());

        /* La pausa casuale aumenta la probabilita' di osservare una race senza mutex. */
        int temp = shared->valore;
        printf("[%d] Valore corrente: %d, lo incremento\n", getpid(), temp);
        sleep(rand() % 2);
        shared->valore = temp + 1;
        printf("[%d] Nuovo valore: %d\n", getpid(), shared->valore);

        if (sem_post(&shared->mutex) == -1) {
            exit(1);
        }

        printf("[%d] Ho lasciato la sezione critica\n", getpid());
    }

    exit(0);
}

int main(void) {
    char name[64];
    sprintf(name, "/so_es06_ex1_%d", getpid());

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
    shared->valore = 0;

    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            figlio(shared);
        }
    }

    for (int i = 0; i < 2; i++) {
        if (wait(NULL) == -1) {
            perror("wait");
            exit(1);
        }
    }

    printf("Valore finale: %d (atteso: 20)\n", shared->valore);

    int result = 1;
    if (shared->valore == 20) {
        result = 0;
    }

    sem_destroy(&shared->mutex);
    munmap(shared, sizeof(shared_t));
    close(fd);
    shm_unlink(name);
    return result;
}
