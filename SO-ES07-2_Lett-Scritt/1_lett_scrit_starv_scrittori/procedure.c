			/*-----IMPLEMENTAZIONE DELLE PROCEDURE-------*/
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#include "header.h"

/*********PROCEDURE DI LETTURA E SCRITTURA*********/

void InizioLettura(Buffer* buf){

	sem_wait(&buf->mutex_numlettori); //Indica ai lettori che sto iniziando a leggere, incremento
				// numlettori in mutua esclusione

	buf->numlettori = buf->numlettori + 1;

	if (buf->numlettori == 1) //se si tratta del primo lettore blocca gli scrittori
		sem_wait(&buf->mutex_lettori_scrittori);

	sem_post(&buf->mutex_numlettori); //Rilascia il mutex per far entrare altri lettori
}

void FineLettura(Buffer* buf){

	sem_wait(&buf->mutex_numlettori); //Indica ai lettori che sto terminando la lettura, decremento
				// numlettori in mutua esclusione

	buf->numlettori = buf->numlettori - 1;

	if (buf->numlettori == 0) //se sono l'ultimo lettore devo rilasciare la risorsa per gli scrittori
		sem_post(&buf->mutex_lettori_scrittori);

	sem_post(&buf->mutex_numlettori); //rilascio il mutex per altri lettori
}


void InizioScrittura(Buffer* buf){
	sem_wait(&buf->mutex_lettori_scrittori);
}


void FineScrittura(Buffer* buf){
	sem_post(&buf->mutex_lettori_scrittori);
}


void Scrittore(Buffer* buf) {

	InizioScrittura(buf);

	/*********Scrittura********/
	struct timeval t1;
	gettimeofday(&t1, NULL); //per avere un valore diverso ad ogni produzione
	msg val = t1.tv_usec;
	buf->messaggio = val;
	sleep(1);
	printf("Valore scritto=<%ld> \n", buf->messaggio);

	FineScrittura(buf);
}


void Lettore(Buffer* buf) {

	InizioLettura(buf);

	/*********Lettura********/
	sleep(1); // per simulare un ritardo di lettura
	printf("Valore letto=<%ld>, numero lettori=%d \n", buf->messaggio, buf->numlettori);

	FineLettura(buf);
}
