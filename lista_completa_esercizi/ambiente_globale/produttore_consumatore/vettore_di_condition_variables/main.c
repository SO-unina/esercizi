#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dvd.h"


void * run_cliente(void * p) {

	struct Monitor * dvd = (struct Monitor *) p;

	int id_copia;

	int id_film;


	/* Affitta i film da 1 a 3 */

	for(id_film=1; id_film<=3; id_film++) {

		id_copia = affitta(dvd, id_film);

		sleep(1);

		restituisci(dvd, id_film, id_copia);
	}

	pthread_exit(NULL);
}


void * run_stampa(void * p) {

	struct Monitor * dvd = (struct Monitor *) p;

	int i;

	for(i=0; i<6; i++) {

		stampa(dvd);

		sleep(1);
	}

	pthread_exit(NULL);
}


int main() {

	pthread_t threads_clienti[4];
	pthread_t thread_stampa;

	pthread_attr_t attr;
    
	int rc;
	int i;

	struct Monitor * dvd = (struct Monitor *)malloc(sizeof(struct Monitor));


	inizializza(dvd);


	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    
	for(i=0; i<4; i++) {
		pthread_create(&threads_clienti[i], &attr, run_cliente, (void *)dvd);
	}

	pthread_create(&thread_stampa, &attr, run_stampa, (void *)dvd);
    


	for(i=0; i<4; i++) {
		pthread_join(threads_clienti[i], NULL);
	}

	pthread_join(thread_stampa, NULL);
    

	distruggi(dvd);
}

