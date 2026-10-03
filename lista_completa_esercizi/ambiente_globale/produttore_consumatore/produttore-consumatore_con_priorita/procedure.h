#ifndef __PROCEDURE_H
#define __PROCEDURE_H

#include <pthread.h>

#define DIM 3

typedef struct{

	int buffer[DIM];
	int testa;
	int coda;
	int count;

	/* mutex del monitor */
	pthread_mutex_t mutex;

	/* condition variables del monitor */
	pthread_cond_t not_full_prio1;
	pthread_cond_t not_full_prio2;
	pthread_cond_t not_empty;

	/* contatori dei thread produttori in attesa, per tipo */
	int num_produttori_alta_prio;
	int num_produttori_bassa_prio;

}PriorityProdCons;


void inizializza_prod_cons(PriorityProdCons* p);
void produci_alta_prio(PriorityProdCons* p, int id, int val);
void produci_bassa_prio(PriorityProdCons* p, int id, int val);
int consuma(PriorityProdCons* p, int id);
void rimuovi_prod_cons(PriorityProdCons* p);

#endif
