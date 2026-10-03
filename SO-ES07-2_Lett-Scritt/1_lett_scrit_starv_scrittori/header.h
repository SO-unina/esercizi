		/*-------HEADER FILE-------------*/

#include <semaphore.h>

/* Nome dell'oggetto di shared memory condiviso */
#define SHM_NAME "/so_es07_lett_scritt_starv_scrittori"

#define NUM_VOLTE 6

typedef long msg;

/* La struttura condivisa contiene il contatore dei lettori, il messaggio
   e i semafori POSIX anonimi usati per la sincronizzazione */
typedef struct {
	int numlettori;
	msg messaggio;
	sem_t mutex_numlettori;
	sem_t mutex_lettori_scrittori;
} Buffer;

void InizioLettura(Buffer*);
void FineLettura(Buffer*);
void InizioScrittura(Buffer*);
void FineScrittura(Buffer*);
void Lettore(Buffer*);
void Scrittore(Buffer*);
