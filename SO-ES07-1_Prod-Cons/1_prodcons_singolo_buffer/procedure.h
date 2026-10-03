#include <semaphore.h>

/* Nome dell'oggetto di shared memory condiviso */
#define SHM_NAME "/so_es07_singolo_buffer"

/* La struttura condivisa contiene il buffer (un solo int)
   e i semafori POSIX anonimi usati per la cooperazione */
struct prodcons {
	sem_t spazio_disponibile;
	sem_t messaggio_disponibile;
	int valore;
};

void produttore(struct prodcons *);
void consumatore(struct prodcons *);
