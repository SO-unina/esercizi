#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"


void produttore(struct prodcons * p) {

	printf("produttore è fermo prima di wait\n");
	sem_wait(&p->spazio_disponibile);
	printf("produttore si sblocca dopo la wait\n");


	sleep(2);

	p->valore = rand() % 100;

	printf("Il valore prodotto = %d\n", p->valore);


	sem_post(&p->messaggio_disponibile);
}

void consumatore(struct prodcons * p) {

	printf("consumatore è fermo prima di wait\n");
	sem_wait(&p->messaggio_disponibile);
	printf("consumatore si sblocca dopo la wait\n");


	sleep(2);
	printf("Il valore consumato = %d\n", p->valore);


	sem_post(&p->spazio_disponibile);
}
