#ifndef __TEATRO__
#define __TEATRO__

#include <semaphore.h>

/* Nome dell'oggetto di shared memory POSIX condiviso tra i due
 * eseguibili (clienti e visualizzatore). I nomi POSIX iniziano
 * con '/' e sono visibili in /dev/shm. */
#define SHM_NAME "/teatro_vett_stato"

typedef struct {
	unsigned int id_cliente;
	unsigned int stato;
} posto;

#define POSTI 80

#define CLIENTI 50

#define LIBERO 0
#define OCCUPATO 1
#define INAGGIORNAMENTO 2

/* Tutto lo stato condiviso (posti, disponibilita e semaforo) vive in
 * un'unica struttura allocata nella shared memory POSIX: il semaforo
 * e' anonimo e process-shared (sem_init con pshared=1). */
typedef struct {
	sem_t mutex;
	int disponibilita;
	posto posti[POSTI];
} teatro_t;

#endif // __TEATRO__
