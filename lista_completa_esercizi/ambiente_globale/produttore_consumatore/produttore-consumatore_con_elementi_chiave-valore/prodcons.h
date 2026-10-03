#ifndef _PRODCONS_H_
#define _PRODCONS_H_

#include <semaphore.h>

/* Nome dell'oggetto di shared memory POSIX (inizia con '/',
 * visibile in /dev/shm). */
#define SHM_NAME "/prodcons_kv"

#define DIM 4

typedef struct {
    int chiave;
    int valore;
} buffer;

/* Tutto lo stato condiviso (vettore di buffer, vettore di stato e
 * semafori) vive in un'unica struttura allocata nella shared memory
 * POSIX. I semafori sono anonimi e process-shared (sem_init con
 * pshared=1): un semaforo "spazio disponibile", tre semafori
 * "messaggio disponibile" (uno per ogni chiave), e un mutex per i
 * produttori.
 *
 * Il semaforo MUTEX_C non è necessario,
 * poiché i consumatori operano su
 * buffer distinti in base alla chiave.
 */
typedef struct {
    sem_t spazio_disp;
    sem_t messaggio_disp_1;
    sem_t messaggio_disp_2;
    sem_t messaggio_disp_3;
    sem_t mutex_p;
    buffer vettore[DIM];
    int stato[DIM];
} prodcons_t;

void produzione(prodcons_t *pc, int chiave, int valore);
int consumazione(prodcons_t *pc, int chiave);

#define LIBERO 0
#define INUSO 1
#define OCCUPATO 2

#endif
