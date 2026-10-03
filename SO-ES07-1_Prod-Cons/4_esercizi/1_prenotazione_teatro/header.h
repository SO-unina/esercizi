#ifndef TEATRO_H
#define TEATRO_H

#include <semaphore.h>

/* Nome dell'oggetto di shared memory POSIX condiviso tra i due
 * eseguibili (clienti e visualizzatore). I nomi POSIX iniziano con '/'
 * e sono visibili in /dev/shm. */
#define TEATRO_SHM_NAME "/so_es07_teatro"

#define POSTI 80
#define CLIENTI 50

#define LIBERO 0
#define OCCUPATO 1
#define INAGGIORNAMENTO 2

typedef struct {
    unsigned int id_cliente;
    unsigned int stato;
} posto;

/* Tutto lo stato condiviso (dati + semaforo) vive nella shared memory:
 * il semaforo e' anonimo e process-shared (sem_init con pshared=1). */
typedef struct {
    sem_t mutex;
    int disponibilita;
    posto posti[POSTI];
} teatro_t;

#endif /* TEATRO_H */
