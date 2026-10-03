#include <semaphore.h>

/* Nome dell'oggetto di shared memory condiviso */
#define SHM_NAME "/so_es07_coda_circolare"

#define DIM_BUFFER 3

#define NUM_PRODUTTORI 5
#define NUM_CONSUMATORI 5


/* La struttura condivisa contiene la coda circolare
   e i semafori POSIX anonimi usati per cooperazione e competizione */
struct prodcons {
    int buffer[DIM_BUFFER];
    int testa;
    int coda;
    sem_t spazio_disponibile;
    sem_t messaggio_disponibile;
    sem_t mutex_p;
    sem_t mutex_c;
};

void produttore(struct prodcons *);
void consumatore(struct prodcons *);
