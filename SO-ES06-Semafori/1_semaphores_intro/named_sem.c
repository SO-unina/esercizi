#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    char name[64];
    sprintf(name, "/so_es06_named_%d", getpid());

    /* Il valore iniziale zero blocca il figlio fino alla sem_post del padre. */
    sem_t *sem = sem_open(name, O_CREAT | O_EXCL, 0600, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Figlio: attendo il semaforo...\n");
        if (sem_wait(sem) == -1) {
            perror("sem_wait");
            exit(1);
        }

        printf("Figlio: semaforo ricevuto.\n");
        sem_close(sem);
        exit(0);
    }

    sleep(1);
    printf("Padre: signal.\n");
    if (sem_post(sem) == -1) {
        perror("sem_post");
        return 1;
    }

    if (wait(NULL) == -1) {
        perror("wait");
        return 1;
    }

    if (sem_close(sem) == -1) {
        perror("sem_close");
        return 1;
    }
    if (sem_unlink(name) == -1) {
        perror("sem_unlink");
        return 1;
    }

    return 0;
}
