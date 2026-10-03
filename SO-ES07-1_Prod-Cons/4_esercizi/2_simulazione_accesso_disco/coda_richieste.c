#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "coda_richieste.h"

/* Nome dell'oggetto shared memory: include il PID per evitare
 * collisioni con altre esecuzioni dello stesso programma. */
static char shm_name[64];

coda_richieste *inizializza_coda(void)
{
    coda_richieste *c;

    sprintf(shm_name, "/so_es07_disco_%d", getpid());

    int fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600);

    if (fd == -1)
    {
        perror("Errore shm_open");
        exit(1);
    }

    /* La dimensione va impostata con ftruncate PRIMA di mmap. */
    if (ftruncate(fd, sizeof(coda_richieste)) == -1)
    {
        perror("Errore ftruncate");
        exit(1);
    }

    c = mmap(NULL, sizeof(coda_richieste), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (c == MAP_FAILED)
    {
        perror("Errore mmap");
        exit(1);
    }

    close(fd);

    /* Semafori per la gestione della cooperazione */
    sem_init(&c->spazio_disp, 1, DIM);
    sem_init(&c->messaggio_disp, 1, 0);

    /* Semafori per la gestione della competizione tra prod e cons */
    sem_init(&c->mutex_p, 1, 1);
    sem_init(&c->mutex_c, 1, 1);

    c->testa = 0;
    c->coda = 0;

    return c;
}

// Agisco da consumatore
void preleva_richiesta(coda_richieste *c, richiesta *r)
{
    sem_wait(&c->messaggio_disp);

    sem_wait(&c->mutex_c); /* Non necessario quando c'e' un solo consumatore */

    printf("[%d] Consumazione in coda: %d\n", getpid(), c->coda);

    r->posizione = c->vettore[c->coda].posizione;
    r->processo = c->vettore[c->coda].processo;

    c->coda = (c->coda + 1) % DIM;

    sem_post(&c->mutex_c); /* Non necessario quando c'e' un solo consumatore */

    sem_post(&c->spazio_disp);
}

// Agisco da produttore
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
    /* Il creatore distrugge i semafori e rimuove il nome della
     * shared memory (l'oggetto sparisce quando nessuno lo mappa piu'). */
    sem_destroy(&c->spazio_disp);
    sem_destroy(&c->messaggio_disp);
    sem_destroy(&c->mutex_p);
    sem_destroy(&c->mutex_c);

    munmap(c, sizeof(coda_richieste));
    shm_unlink(shm_name);
}
