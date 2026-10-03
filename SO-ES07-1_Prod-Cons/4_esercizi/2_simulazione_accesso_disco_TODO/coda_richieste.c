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

    // Creo l'oggetto di shared memory POSIX (fallire se esiste gia')
    int fd = shm_open(/* TODO */);

    if (fd == -1)
    {
        perror("Errore shm_open");
        exit(1);
    }

    // Imposto la dimensione dell'oggetto PRIMA di mmap
    if (ftruncate(/* TODO */) == -1)
    {
        perror("Errore ftruncate");
        exit(1);
    }

    // Mappo l'oggetto nello spazio di indirizzamento
    c = mmap(/* TODO */);

    if (c == MAP_FAILED)
    {
        perror("Errore mmap");
        exit(1);
    }

    close(fd);

    // Semafori per la gestione della cooperazione
    // (anonimi, process-shared: pshared=1)
    sem_init(/* TODO */);
    sem_init(/* TODO */);

    // Semafori per la gestione della competizione tra prod e cons
    sem_init(/* TODO */);
    sem_init(/* TODO */);

    c->testa = 0;
    c->coda = 0;

    return c;
}

// Agisco da consumatore
void preleva_richiesta(coda_richieste *c, richiesta *r)
{
    /* TODO: attendere una richiesta disponibile ed entrare in
     * mutua esclusione (sem_wait sui semafori opportuni) */

    printf("[%d] Consumazione in coda: %d\n", getpid(), c->coda);

    r->posizione = c->vettore[c->coda].posizione;
    r->processo = c->vettore[c->coda].processo;

    c->coda = (c->coda + 1) % DIM;

    /* TODO: uscire dalla mutua esclusione e segnalare lo spazio
     * liberato (sem_post sui semafori opportuni) */
}

// Agisco da produttore
void inserisci_richiesta(coda_richieste *c, richiesta *r)
{
    /* TODO: attendere spazio disponibile ed entrare in mutua
     * esclusione (sem_wait sui semafori opportuni) */

    printf("[%d] Produzione in testa: %d\n", getpid(), c->testa);

    c->vettore[c->testa].posizione = r->posizione;
    c->vettore[c->testa].processo = r->processo;

    c->testa = (c->testa + 1) % DIM;

    /* TODO: uscire dalla mutua esclusione e segnalare la nuova
     * richiesta disponibile (sem_post sui semafori opportuni) */
}

void rimuovi_coda(coda_richieste *c)
{
    // Distruggo i semafori e rimuovo il nome della shared memory
    sem_destroy(/* TODO */);
    sem_destroy(/* TODO */);
    sem_destroy(/* TODO */);
    sem_destroy(/* TODO */);

    munmap(/* TODO */);
    shm_unlink(/* TODO */);
}
