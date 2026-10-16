#include "shared_data.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

/* Il writer e' un programma nuovo, lanciato dal master con exec: non ha
 * ereditato alcun mapping, quindi ritrova la risorsa PER NOME.
 * Apre senza O_CREAT: l'oggetto deve gia' esistere (lo crea il master). */

int main(void) {
    int fd = shm_open(SHM_NAME, O_RDWR, 0);
    if (fd == -1) {
        perror("shm_open (avviare tramite ./master)");
        exit(1);
    }

    shared_data_t *data = mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);

    data->value = 1234;
    sprintf(data->text, "Scritto dal writer PID %d", getpid());

    printf("[writer %d] Ho scritto value=%d\n", getpid(), data->value);

    /* Il writer NON fa shm_unlink: chi usa la risorsa si limita a
     * rilasciare il proprio mapping; il nome lo rimuove il creatore. */
    munmap(data, sizeof(shared_data_t));

    exit(0);
}
