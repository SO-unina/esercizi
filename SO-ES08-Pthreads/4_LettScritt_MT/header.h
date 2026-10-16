			/*-----HEADER FILE-----*/
#include <pthread.h>

#ifndef HEADER_H
#define HEADER_H

/*messaggio*/
typedef long msg;


struct LettScritt {

	/* mutex del monitor */
	pthread_mutex_t mutex;

	/* condition variables del monitor */
	pthread_cond_t ok_lett_cv;
	pthread_cond_t ok_scritt_cv;

	/* variabili di utilità per la sincronizzazione */
	int num_lettori;	// lettori attivi
	int num_scrittori;	// scrittori attivi
	int num_lettori_wait;	// lettori in attesa
	int num_scrittori_wait;	// scrittori in attesa

	/* buffer condiviso */
	msg mess;
};


void * Lettore(void *);
void * Scrittore(void *);

void Scrivi(struct LettScritt *ls, msg m);
msg Leggi(struct LettScritt *ls);

void InizioLettura(struct LettScritt *ls);
void InizioScrittura(struct LettScritt *ls);
void FineLettura(struct LettScritt *ls);
void FineScrittura(struct LettScritt *ls);

#endif
