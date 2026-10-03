#ifndef _CODA_RICHIESTE_H_
#define _CODA_RICHIESTE_H_

#include <sys/types.h>

#include <semaphore.h>

#define DIM 10

typedef struct {
    unsigned int posizione;
    pid_t processo;
} richiesta;

/* La coda circolare, i suoi indici e i semafori che la proteggono
 * vivono tutti nella stessa shared memory POSIX: i semafori sono
 * anonimi e process-shared (sem_init con pshared=1). */
typedef struct {
    sem_t spazio_disp;    /* conta gli slot liberi (init: DIM)   */
    sem_t messaggio_disp; /* conta le richieste presenti (init: 0) */
    sem_t mutex_p;        /* mutua esclusione tra produttori     */
    sem_t mutex_c;        /* mutua esclusione tra consumatori
                           * (opzionale: qui c'e' un solo consumatore) */
    richiesta vettore[DIM];
    int testa;
    int coda;
} coda_richieste;

coda_richieste *inizializza_coda(void);
void preleva_richiesta(coda_richieste *c, richiesta *r);
void inserisci_richiesta(coda_richieste *c, richiesta *r);
void rimuovi_coda(coda_richieste *c);

#endif
