#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "buffer.h"

/* Apre (senza O_CREAT: gli oggetti devono gia' esistere, creati da
 * main-padre) e mappa un oggetto di shared memory POSIX. */
static void * apri_shm(const char *nome, size_t dim)
{
    int fd = shm_open(nome, O_RDWR, 0);

    if (fd < 0)
    {
        perror("Errore accesso SHM (avviare prima ./main-padre)");
        exit(1);
    }

    void *p = mmap(NULL, dim, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (p == MAP_FAILED)
    {
        perror("Errore mmap SHM");
        exit(1);
    }

    close(fd);

    return p;
}

int main()
{

    buffer *buf1;
    buffer *buf2;
    sincro *sem;

    buf1 = apri_shm(SHM_BUF1, sizeof(buffer));

    buf2 = apri_shm(SHM_BUF2, sizeof(buffer));

    sem = apri_shm(SHM_SINCRO, sizeof(sincro));

    srand(time(NULL));

    for (int i = 0; i < 5; i++)
    {

        int valore = rand() % 10;

        produzione(sem, buf1, buf2, valore);
    }

    exit(0);
}
