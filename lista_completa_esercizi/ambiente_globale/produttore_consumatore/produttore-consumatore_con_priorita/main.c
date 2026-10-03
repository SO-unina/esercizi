#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUM_PRODUTTORI_1  1
#define NUM_PRODUTTORI_2  3
#define NUM_CONSUMATORI  1

#define NUM_PRODUZIONI_1  3
#define NUM_PRODUZIONI_2  3
#define NUM_CONSUMAZIONI  12

#define NUM_THREADS (NUM_PRODUTTORI_1 + NUM_PRODUTTORI_2 + NUM_CONSUMATORI)


/* argomenti passati ad ogni thread: identificativo del thread
   e puntatore alla struttura condivisa */
typedef struct {
	int id;
	PriorityProdCons * p;
} ArgomentiThread;


void * produttore_alta_priorita(void * arg)
{
	ArgomentiThread * a = (ArgomentiThread *) arg;

	printf("Sono un produttore di tipo 1. Il mio id è %d \n", a->id);

	for(int i=0; i < NUM_PRODUZIONI_1; i++) {

		int value = rand() % 12;

		produci_alta_prio(a->p, a->id, value);

		sleep(2);
	}

	pthread_exit(NULL);
}


void * produttore_bassa_priorita(void * arg)
{
	ArgomentiThread * a = (ArgomentiThread *) arg;

	printf("Sono un produttore di tipo 2. Il mio id è %d \n", a->id);

	for(int i=0; i < NUM_PRODUZIONI_2; i++) {

		int value = 13 + (rand() % 12);

		produci_bassa_prio(a->p, a->id, value);

		sleep(1);
	}

	pthread_exit(NULL);
}


void * consumatore(void * arg)
{
	ArgomentiThread * a = (ArgomentiThread *) arg;

	printf("Sono un consumatore. Il mio id è %d \n", a->id);

	for(int i=0; i < NUM_CONSUMAZIONI; i++) {

		sleep(1);

		consuma(a->p, a->id);
	}

	pthread_exit(NULL);
}


int main()
{
	pthread_t threads[NUM_THREADS];
	ArgomentiThread argomenti[NUM_THREADS];
	int k = 0;

	/* lo stato condiviso è una normale struttura allocata nel main:
	   i thread condividono lo stesso spazio di indirizzamento */
	PriorityProdCons * p = malloc(sizeof(PriorityProdCons));

	if (p == NULL) {
		perror("Errore allocazione struttura condivisa");
		exit(1);
	}

	inizializza_prod_cons(p);

	srand(time(NULL));


	// avvio produttori e consumatori

	for (int i=0; i<NUM_PRODUTTORI_1; i++, k++) {

		argomenti[k].id = k;
		argomenti[k].p = p;

		pthread_create(&threads[k], NULL, produttore_alta_priorita, (void *) &argomenti[k]);
	}

	for (int i=0; i<NUM_PRODUTTORI_2; i++, k++) {

		argomenti[k].id = k;
		argomenti[k].p = p;

		pthread_create(&threads[k], NULL, produttore_bassa_priorita, (void *) &argomenti[k]);
	}

	for (int i=0; i<NUM_CONSUMATORI; i++, k++) {

		argomenti[k].id = k;
		argomenti[k].p = p;

		pthread_create(&threads[k], NULL, consumatore, (void *) &argomenti[k]);
	}


	for (k=0; k<NUM_THREADS; k++) {

		pthread_join(threads[k], NULL);

		printf("Thread n.ro %d è terminato\n", k);
	}

	printf("Thread main terminato\n");

	// deallocazione delle risorse di sincronizzazione
	rimuovi_prod_cons(p);

	free(p);

	return 0;
}
