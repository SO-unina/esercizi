#include "shared_data.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    char name[64];
    sprintf(name, "/so_es05_single_%d", getpid());

    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(fd, sizeof(shared_data_t)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    shared_data_t *data = mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // shared_data_t *data = mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0); // cosa cambia con MAP_PRIVATE?

    // il figlio riceve una copia dello spazio di memoria ma le sue modifiche non sono visibili al padre! 

    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    data->value = 10;
    sprintf(data->text, "Valore iniziale impostato dal padre");

    printf("PADRE: il valore è: %d\n", data->value);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        /* Dopo fork, il mapping MAP_SHARED continua a riferirsi allo stesso oggetto. */
        data->value += 32;
        sprintf(data->text, "Area modificata dal figlio PID %d\n", getpid());

        printf("FIGLIO: il valore è: %d\n", data->value);

        if (munmap(data, sizeof(shared_data_t)) == -1) {
            perror("munmap figlio");
            exit(1);
        }
        if (close(fd) == -1) {
            perror("close figlio");
            exit(1);
        }
        exit(0);
    }

    /* Esiste un solo figlio, quindi wait e' sufficiente. */
    if (wait(NULL) == -1) {
        perror("wait");
        exit(1);
    }

    printf("PADRE: value=%d, text=%s\n", data->value, data->text);

    data->value += 32;

    printf("PADRE: Ho modificato di nuovo il valore, adesso è: %d\n", data->value);

    if (munmap(data, sizeof(shared_data_t)) == -1) {
        perror("munmap padre");
        exit(1);
    }
    if (close(fd) == -1) {
        perror("close");
        exit(1);
    }
    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
        exit(1);
    }

    return 0;
}
