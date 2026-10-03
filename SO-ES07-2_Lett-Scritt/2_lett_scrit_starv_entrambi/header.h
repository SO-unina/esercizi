#ifndef _HEADER_H_
#define _HEADER_H_

#include <semaphore.h>

/* Nome dell'oggetto di shared memory condiviso */
#define SHM_NAME "/so_es07_lett_scritt"

#define NUM_VOLTE 3

typedef long msg;


/* La struttura condivisa contiene i contatori, il messaggio
   e i semafori POSIX anonimi usati per la sincronizzazione */
typedef struct {
	int numlettori;
	int numscrittori;
	msg messaggio;
	sem_t mutex_lettori_scrittori;
	sem_t mutex_numscrittori;
	sem_t mutex_numlettori;
	sem_t mutex_scrittori;
} Buffer;



void InizioLettura(Buffer*);
void FineLettura(Buffer*);
void InizioScrittura(Buffer*);
void FineScrittura(Buffer*);
void Lettore(Buffer*);
void Scrittore(Buffer*);

#endif //_HEADER_H_
