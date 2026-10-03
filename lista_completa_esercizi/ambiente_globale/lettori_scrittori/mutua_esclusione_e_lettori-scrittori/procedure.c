#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"

void inizio_lettura(esame_t* esame){
	sem_wait(&esame->mutex);
	esame->numero_lettori = esame->numero_lettori+1;
	printf("DEBUG: Numero lettori %d\n",esame->numero_lettori);
	if(esame->numero_lettori == 1)
		sem_wait(&esame->appello);
	sem_post(&esame->mutex);
}

void fine_lettura(esame_t* esame){
	sem_wait(&esame->mutex);
	esame->numero_lettori = esame->numero_lettori - 1;
	printf("DEBUG: Numero lettori %d\n",esame->numero_lettori);
	if(esame->numero_lettori == 0)
		sem_post(&esame->appello);
	sem_post(&esame->mutex);
}

void inizio_scrittura(esame_t* esame){
	sem_wait(&esame->appello);
	printf("DEBUG: Inizio scrittura\n");
}

void fine_scrittura(esame_t* esame){
	sem_post(&esame->appello);
	printf("DEBUG: Fine scrittura\n");
}

void accedi_prenotati(esame_t* esame){
	sem_wait(&esame->prenotati);
	printf("DEBUG: Inizio scrittura prenotati\n");
}

void lascia_prenotati(esame_t* esame){
	sem_post(&esame->prenotati);
	printf("DEBUG: Fine scrittura prenotati\n");
}
