#include "shared_data.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

/* Anche il reader riapre la risorsa per nome. Deve solo leggere, quindi
 * apre in sola lettura e mappa con PROT_READ. */

int main(void) {
    int fd = shm_open(SHM_NAME, O_RDONLY, 0);
    if (fd == -1) {
        perror("shm_open (avviare tramite ./master)");
        exit(1);
    }

    shared_data_t *data = mmap(NULL, sizeof(shared_data_t), PROT_READ, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);

    printf("[reader %d] Ho letto value=%d, text=%s\n", getpid(), data->value, data->text);

    /* Come il writer: munmap del proprio mapping, nessuna shm_unlink. */
    munmap(data, sizeof(shared_data_t));

    exit(0);
}
