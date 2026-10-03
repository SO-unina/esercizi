#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>

#include "coda_richieste.h"

coda_richieste *inizializza_coda()
{
    int shm_fd;
    coda_richieste *c;

    /* Elimina un eventuale oggetto rimasto da una precedente
     * esecuzione, cosi' la shm_open con O_EXCL non fallisce. */
    shm_unlink(SHM_NAME);

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0644);

    if (shm_fd < 0)
    {
        perror("Errore creazione SHM coda richieste");
        exit(1);
    }

    /* La dimensione dell'oggetto (inizialmente 0) va impostata con
     * ftruncate PRIMA di mmap. */
    if (ftruncate(shm_fd, sizeof(coda_richieste)) < 0)
    {
        perror("Errore ftruncate SHM");
        exit(1);
    }

    c = mmap(NULL, sizeof(coda_richieste), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (c == MAP_FAILED)
    {
        perror("Errore mmap SHM");
        exit(1);
    }

    close(shm_fd);

    /* Semafori anonimi condivisi tra processi: pshared = 1 e
     * collocazione nella struttura in shared memory (la mappatura
     * MAP_SHARED viene ereditata dai figli attraverso la fork). */

    sem_init(&c->spazio_disp, 1, DIM);
    sem_init(&c->messaggio_disp, 1, 0);
    sem_init(&c->mutex_p, 1, 1);
    sem_init(&c->mutex_c, 1, 1);

    c->testa = 0;
    c->coda = 0;

    return c;
}

void preleva_richiesta(coda_richieste *c, richiesta *r)
{
    sem_wait(&c->messaggio_disp);

    sem_wait(&c->mutex_c); /* Non necessario quando c'è un solo consumatore */

    printf("[%d] Consumazione in coda: %d\n", getpid(), c->coda);

    r->posizione = c->vettore[c->coda].posizione;
    r->processo = c->vettore[c->coda].processo;

    c->coda = (c->coda + 1) % DIM;

    sem_post(&c->mutex_c); /* Non necessario quando c'è un solo consumatore */

    sem_post(&c->spazio_disp);
}

void inserisci_richiesta(coda_richieste *c, richiesta *r)
{
    sem_wait(&c->spazio_disp);

    sem_wait(&c->mutex_p);

    printf("[%d] Produzione in testa: %d\n", getpid(), c->testa);

    c->vettore[c->testa].posizione = r->posizione;
    c->vettore[c->testa].processo = r->processo;

    c->testa = (c->testa + 1) % DIM;

    sem_post(&c->mutex_p);

    sem_post(&c->messaggio_disp);
}

void rimuovi_coda(coda_richieste *c)
{
    /* Cleanup: si distruggono i semafori, poi si rimuovono la
     * mappatura e il nome dell'oggetto da /dev/shm (shm_unlink). */
    sem_destroy(&c->spazio_disp);
    sem_destroy(&c->messaggio_disp);
    sem_destroy(&c->mutex_p);
    sem_destroy(&c->mutex_c);

    munmap(c, sizeof(coda_richieste));
    shm_unlink(SHM_NAME);
}
