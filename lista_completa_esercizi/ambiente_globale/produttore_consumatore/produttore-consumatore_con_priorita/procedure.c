/*IMPLEMENTAZIONE DELLE PROCEDURE*/

#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define NOT_FULL_1 0
#define NOT_FULL_2 1
#define NOT_EMPTY 2
#define NUM_VAR_COND 3

void inizializza_prod_cons(PriorityProdCons* p){
	p->testa = 0;
	p->coda = 0;
	p->count = 0;
	p->num_produttori_alta_prio = 0;

	init_monitor(&p->m,NUM_VAR_COND);
}

void rimuovi_prod_cons(PriorityProdCons* p){

	remove_monitor(&p->m);
}

//In questo caso la semantica è signal and continue
void produci_alta_prio(PriorityProdCons* p, int value){

	enter_monitor(&p->m);

	printf("Produttore tipo 1 con pid %d accede al monitor\n", getpid());

	// Attendo che il vettore non sia pieno
	while(p->count == DIM) {
		p->num_produttori_alta_prio++;
		wait_condition(&p->m,NOT_FULL_1);
		p->num_produttori_alta_prio--;
	}

	// Produzione

	p->buffer[p->testa] = value;
	p->testa = (p->testa + 1) % DIM;
	p->count++;

	printf("Produttore tipo 1 con pid %d ha prodotto %d\n",getpid(),value);


	// Riattivazione di un consumatore
	signal_condition(&p->m,NOT_EMPTY);


	leave_monitor(&p->m);

}

void produci_bassa_prio(PriorityProdCons* p, int value){

	enter_monitor(&p->m);

	printf("Produttore tipo 2 con pid %d accede al monitor\n", getpid());


	//Attendo che il vettore NON sia pieno, è che NON vi siano altri produttori ad alta priorità in attesa

	while(p->count == DIM || p->num_produttori_alta_prio > 0) {
		wait_condition(&p->m,NOT_FULL_2);
	}


	// Produzione

	p->buffer[p->testa] = value;
	p->testa = (p->testa + 1) % DIM;
	p->count++;

	printf("Produttore tipo 2 con pid %d ha prodotto %d\n", getpid(), value);


	// Riattivazione di un consumatore
	signal_condition(&p->m,NOT_EMPTY);


	leave_monitor(&p->m);

}

int consuma(PriorityProdCons* p){

	int value;

	enter_monitor(&p->m);

	printf("Consumatore con pid %d accede al monitor\n", getpid());


	//Se non c'è nulla nel vettore, attendo che qualcuno produca
	while(p->count == 0){
		wait_condition(&p->m,NOT_EMPTY);
	}


	// Consumazione

	value = p->buffer[p->coda];
	p->coda = (p->coda + 1) % DIM;
	p->count--;

	printf("Consumatore con pid %d ha consumato %d\n", getpid(), value);


	printf("Processi in coda nella variabile NOT_FULL_1 %d\n",queue_condition(&p->m,NOT_FULL_1));
	printf("Processi in coda nella variabile NOT_FULL_2 %d\n",queue_condition(&p->m,NOT_FULL_2));

	// Si riattiva un produttore di tipo 1 se presente,
	// altrimenti si riattiva un produttore di tipo 2
	if(p->num_produttori_alta_prio > 0)
		signal_condition(&p->m,NOT_FULL_1);
	else
		signal_condition(&p->m,NOT_FULL_2);


	leave_monitor(&p->m);


	return value;

}
