#include "shared_data.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void) {
    /* Il reader apre un oggetto gia' creato dal writer. */
    int fd = shm_open(SHM_NAME, O_RDWR, 0);
    if (fd == -1) {
        perror("shm_open: avviare prima writer");
        exit(1);
    }

    shared_data_t *data = mmap(NULL, sizeof(shared_data_t), PROT_READ, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("value=%d, text=%s\n", data->value, data->text);

    // che succede se provo a scrivere senza | PROT_WRITE nella mmap e O_RDWR nella shm_open?
    // data->value = 5678;

    /*
        1. shm_open con O_RDONLY + mmap con PROT_READ | PROT_WRITE
            -> mmap: Permission denied
        2. shm_open con O_RDWR + mmap con PROT_READ
            -> Errore di segmentazione (core dump creato)
        3. shm_open con O_RDWR + mmap con PROT_READ | PROT_WRITE
            -> scrittura ok

    */

    printf("value=%d, text=%s\n", data->value, data->text);

    printf("Dati letti. Premere Invio per terminare.\n", getpid());
    getchar();

    if (munmap(data, sizeof(shared_data_t)) == -1) {
        perror("munmap");
        exit(1);
    }
    if (close(fd) == -1) {
        perror("close");
        exit(1);
    }

    /* Il reader non esegue shm_unlink: l'oggetto appartiene al writer. */
    return 0;
}
