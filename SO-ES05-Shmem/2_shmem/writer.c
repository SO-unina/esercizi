#include "shared_data.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void) {
    /* Elimina un eventuale oggetto rimasto da una precedente esecuzione. */
    if (shm_unlink(SHM_NAME) == -1) {
        /* ENOENT e' normale alla prima esecuzione; non e' necessario segnalarlo. */
    }

    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(fd, sizeof(shared_data_t)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    shared_data_t *data = mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    data->value = 1234;
    sprintf(data->text, "Messaggio dello scrittore PID %d", getpid());

    printf("Dati pubblicati. Avviare ./reader e premere Invio per terminare.\n");
    getchar();

    if (munmap(data, sizeof(shared_data_t)) == -1) {
        perror("munmap");
        exit(1);
    }
    if (close(fd) == -1) {
        perror("close");
        exit(1);
    }
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(1);
    }

    return 0;
}
