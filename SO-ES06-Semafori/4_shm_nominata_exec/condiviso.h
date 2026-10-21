#include <semaphore.h>

/* Nome dell'oggetto di shared memory condiviso */
#define SHM_NAME "/so_es06_caso2"

#define INCREMENTI 5

/* La struct contiene i dati E il semaforo anonimo */
typedef struct {
    sem_t mutex;
    int contatore;
} condiviso_t;
