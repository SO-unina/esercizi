#ifndef _DVD_H_
#define _DVD_H_


#include <pthread.h>


#define DISPONIBILE 0
#define AFFITTATO 1

#define NUM_DVD 6


struct DVD {
	int identificativo_film;	// un intero tra 1 e 3
	int identificativo_copia;	// un intero tra 1 e 2
	int stato;			// DISPONIBILE=0, oppure AFFITTATO=1
};


struct Monitor {
	struct DVD dvd[NUM_DVD];	// vettore di DVD da gestire
	pthread_mutex_t	mutex;		// mutex del monitor
	pthread_cond_t cv[3];		// una CV per ogni film da gestire
};


int affitta(struct Monitor * m, int id_film);
void restituisci(struct Monitor * m, int id_film, int id_copia);
void stampa(struct Monitor * m);
void inizializza(struct Monitor * m);
void distruggi(struct Monitor * m);
int ricerca_dvd(struct DVD vettore [], int id_film);


#endif // _DVD_H_

