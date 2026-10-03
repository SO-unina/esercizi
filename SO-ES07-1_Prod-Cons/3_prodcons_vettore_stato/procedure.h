#include <semaphore.h>

/* Nome dell'oggetto di shared memory condiviso */
#define SHM_NAME "/so_es07_vettore_stato"

#define DIM_BUFFER 3

#define NUM_PRODUTTORI 5
#define NUM_CONSUMATORI 5

#define BUFFER_VUOTO 0
#define BUFFER_INUSO 1
#define BUFFER_PIENO 2


/* La struttura condivisa contiene il pool di buffer, il vettore di stato
   e i semafori POSIX anonimi usati per cooperazione e competizione */
struct prodcons {
    int buffer[DIM_BUFFER];
    int stato[DIM_BUFFER];
    sem_t spazio_disponibile;
    sem_t messaggio_disponibile;
    sem_t mutex_p;
    sem_t mutex_c;
};

void produttore(struct prodcons *);
void consumatore(struct prodcons *);
