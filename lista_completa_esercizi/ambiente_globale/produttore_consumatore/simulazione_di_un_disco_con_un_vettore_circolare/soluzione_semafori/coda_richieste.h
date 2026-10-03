#ifndef _CODA_RICHIESTE_H_
#define _CODA_RICHIESTE_H_

#include <sys/types.h>
#include <semaphore.h>

/* Nome dell'oggetto di shared memory POSIX (inizia con '/',
 * visibile in /dev/shm). */
#define SHM_NAME "/disco_coda"

typedef struct
{
    unsigned int posizione;
    pid_t processo;
} richiesta;

#define DIM 10

/* Tutto lo stato condiviso (coda circolare, indici e semafori) vive in
 * un'unica struttura allocata nella shared memory POSIX. I semafori
 * sono anonimi e process-shared (sem_init con pshared=1). */
typedef struct
{
    sem_t spazio_disp;
    sem_t messaggio_disp;
    sem_t mutex_p;
    sem_t mutex_c;
    richiesta vettore[DIM];
    int testa;
    int coda;
} coda_richieste;

/* Nota: mutex_c è opzionale in questo esercizio,
 * c'è un solo consumatore */

coda_richieste * inizializza_coda();
void preleva_richiesta(coda_richieste *c, richiesta * r);
void inserisci_richiesta(coda_richieste *c, richiesta * r);
void rimuovi_coda(coda_richieste *c);

#endif
