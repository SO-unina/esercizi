#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "buffer.h"

/* Crea un oggetto di shared memory POSIX, ne imposta la dimensione
 * (ftruncate va chiamata PRIMA di mmap) e lo mappa. Il creatore usa
 * O_CREAT|O_EXCL, dopo una shm_unlink preventiva che elimina eventuali
 * residui di esecuzioni precedenti. */
static void * crea_shm(const char *nome, size_t dim)
{
    shm_unlink(nome);

    int fd = shm_open(nome, O_CREAT | O_EXCL | O_RDWR, 0644);

    if (fd < 0)
    {
        perror("Errore creazione SHM");
        exit(1);
    }

    if (ftruncate(fd, dim) < 0)
    {
        perror("Errore ftruncate SHM");
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

    buf1 = crea_shm(SHM_BUF1, sizeof(buffer));

    buf2 = crea_shm(SHM_BUF2, sizeof(buffer));

    sem = crea_shm(SHM_SINCRO, sizeof(sincro));

    buf1->stato = LIBERO;
    buf2->stato = LIBERO;

    /* Semafori anonimi condivisi tra processi: pshared = 1 e
     * collocazione in shared memory. */

    if (sem_init(&sem->spazio_disp, 1, 2) < 0)
    {
        perror("Errore inizializzazione semafori");
        exit(1);
    }

    if (sem_init(&sem->messaggio_disp, 1, 0) < 0)
    {
        perror("Errore inizializzazione semafori");
        exit(1);
    }

    pid_t pid1 = fork();

    if (pid1 < 0)
    {
        perror("Errore creazione produttore");
        exit(1);
    }
    else if (pid1 == 0)
    {
        /* figlio */

        execl("./main-produttore", "main-produttore", NULL);

        perror("Errore exec produttore");
        exit(1);
    }

    pid_t pid2 = fork();

    if (pid2 < 0)
    {
        perror("Errore creazione consumatore");
        exit(1);
    }
    else if (pid2 == 0)
    {
        /* figlio */

        execl("./main-consumatore", "main-consumatore", NULL);

        perror("Errore exec consumatore");
        exit(1);
    }

    for (int i = 0; i < 2; i++)
    {
        wait(NULL);
    }

    /* Cleanup: il creatore distrugge i semafori, rimuove le mappature
     * e infine rimuove i nomi degli oggetti da /dev/shm (shm_unlink). */

    sem_destroy(&sem->spazio_disp);
    sem_destroy(&sem->messaggio_disp);

    munmap(buf1, sizeof(buffer));
    munmap(buf2, sizeof(buffer));
    munmap(sem, sizeof(sincro));

    shm_unlink(SHM_BUF1);
    shm_unlink(SHM_BUF2);
    shm_unlink(SHM_SINCRO);
}
