#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "buffer.h"

int main()
{
    printf("[%d] Avvio lettore\n", getpid());


    /* Apre l'oggetto di shared memory gia' creato da main_padre:
     * senza O_CREAT, se l'oggetto non esiste si ha errore. */
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0);

    if(shm_fd < 0) {
        perror("Errore apertura shared memory");
        exit(1);
    }

    buffer * b = mmap(NULL, sizeof(buffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if(b == MAP_FAILED) {
        perror("Errore mmap shared memory");
        exit(1);
    }

    close(shm_fd);

    /* I semafori sono campi della struttura in shared memory:
     * gia' inizializzati dal padre, sono subito pronti all'uso. */

    srand(getpid());

    for(int i=0; i<5; i++) {

        int val_1 = 0;
        int val_2 = 0;

        leggi_buffer(b, &val_1, &val_2);
    }

    munmap(b, sizeof(buffer));

    return 0;

}
