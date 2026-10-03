			/*------IMPLEMENTAZIONE DELLE PROCEDURE--------*/

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "header.h"

//Procedure di inizio e fine lettura

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

	sem_post(&buf->mutex_numlettori); //rilascio il mutex per altri lettori che vogliono iniziare la lettura
}

//Procedure di inizio e fine scrittura

void InizioScrittura(Buffer* buf){

	sem_wait(&buf->mutex_numscrittori); //Indica agli scrittori che sto iniziando a scrivere, incremento
				// numscrittori in mutua esclusione
	buf->numscrittori = buf->numscrittori + 1;

	if (buf->numscrittori == 1) // se si tratta del primo scrittore blocca i lettori
		sem_wait(&buf->mutex_lettori_scrittori);

	sem_post(&buf->mutex_numscrittori); //Rilascia il mutex per far entrare altri scrittori per potersi mettere in attesa

	sem_wait(&buf->mutex_scrittori); //Blocco eventuali scrittori per la scrittura vera e propria
}

void FineScrittura(Buffer* buf){

	sem_post(&buf->mutex_scrittori); //Rilascio il mutex per gli scrittori che devono scrivere

	sem_wait(&buf->mutex_numscrittori); //Indica agli scrittori che sto terminando la scrittura, decremento
					// numscrittori in mutua esclusione

	buf->numscrittori = buf->numscrittori - 1;

	if (buf->numscrittori == 0) //se sono l'ultimo scrittore devo rilasciare la risorsa per i lettori
		sem_post(&buf->mutex_lettori_scrittori);

	sem_post(&buf->mutex_numscrittori); //rilascio il mutex per altri scrittori che vogliono iniziare la scrittura
}



void Scrittore(Buffer *buf){

	InizioScrittura(buf);

	struct timeval t1;
	gettimeofday(&t1, NULL);    //valore diverso ad ogni scrittura
	buf->messaggio = t1.tv_usec;
	sleep(1);
	printf("Valore scritto: <%ld> \n", buf->messaggio);

	FineScrittura(buf);
}

void Lettore (Buffer* buf) {

	InizioLettura(buf);

	/*********Lettura********/
	sleep(1); // per simulare un ritardo di lettura
	printf("Valore letto=<%ld>, numero lettori=%d \n", buf->messaggio, buf->numlettori);

	FineLettura(buf);
}
