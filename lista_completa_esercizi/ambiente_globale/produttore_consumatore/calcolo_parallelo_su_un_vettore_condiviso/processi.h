#ifndef _PROCESSI_H_
#define _PROCESSI_H_

#include <semaphore.h>

#define NUM_PROCESSI 10
#define NUM_ELEMENTI 10000

/* Nome dell'oggetto di shared memory POSIX (inizia con '/',
 * visibile in /dev/shm). */
#define SHM_NAME "/calc_min_vett"

/* Tutto lo stato condiviso (vettore, buffer e semafori) vive in
 * un'unica struttura allocata nella shared memory POSIX. I semafori
 * sono anonimi e process-shared (sem_init con pshared=1); il loro
 * significato dipende dalla variante dell'esercizio (mutua esclusione
 * oppure produttore-consumatore). */
typedef struct {
    sem_t semafori[2];
    int buffer;
    int vettore[NUM_ELEMENTI];
} condiviso_t;

int inizializza_semafori(condiviso_t *c);
void distruggi_semafori(condiviso_t *c);

void figlio(condiviso_t *c, int elemento_iniziale, int qta_elementi);

void padre(condiviso_t *c);

#endif /* _PROCESSI_H_ */
