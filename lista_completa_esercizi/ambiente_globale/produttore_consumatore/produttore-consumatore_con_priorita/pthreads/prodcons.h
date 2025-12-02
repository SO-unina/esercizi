#ifndef __PROCEDURE_H
#define __PROCEDURE_H

#include <pthread.h>

#define DIM 3

typedef struct{

	int buffer[DIM];
	int testa;
	int coda;
    int count;

	pthread_mutex_t mutex;
    
	pthread_cond_t not_full_prio1;
	pthread_cond_t not_full_prio2;
    pthread_cond_t not_empty;
    
    int threads_prio_1;
    int threads_prio_2;
    
} PriorityProdCons;


void inizializza_prod_cons(PriorityProdCons * p);
void produci_alta_prio(PriorityProdCons * p, int value);
void produci_bassa_prio(PriorityProdCons * p, int value);
int consuma(PriorityProdCons * p);
void rimuovi_prod_cons(PriorityProdCons * p);

#endif
