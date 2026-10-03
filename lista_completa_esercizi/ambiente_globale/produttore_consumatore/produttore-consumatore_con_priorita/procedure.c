/*IMPLEMENTAZIONE DELLE PROCEDURE*/

/* Il costrutto monitor è realizzato direttamente con i Pthreads:
 * un mutex (mutua esclusione del monitor) e tre condition variables.
 * La semantica è SIGNAL-AND-CONTINUE: il thread segnalante prosegue
 * l'esecuzione nel monitor, quindi ogni attesa va realizzata con un
 * ciclo while che ricontrolla la condizione logica al risveglio. */

#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void inizializza_prod_cons(PriorityProdCons* p){
	p->testa = 0;
	p->coda = 0;
	p->count = 0;
	p->num_produttori_alta_prio = 0;
	p->num_produttori_bassa_prio = 0;

	pthread_mutex_init( &(p->mutex), NULL );
	pthread_cond_init( &(p->not_full_prio1), NULL );
	pthread_cond_init( &(p->not_full_prio2), NULL );
	pthread_cond_init( &(p->not_empty), NULL );
}

void rimuovi_prod_cons(PriorityProdCons* p){

	pthread_mutex_destroy( &(p->mutex) );
	pthread_cond_destroy( &(p->not_full_prio1) );
	pthread_cond_destroy( &(p->not_full_prio2) );
	pthread_cond_destroy( &(p->not_empty) );
}

//In questo caso la semantica è signal and continue
void produci_alta_prio(PriorityProdCons* p, int id, int value){

	/* Ingresso nel monitor */
	pthread_mutex_lock( &(p->mutex) );

	printf("Produttore tipo 1 con id %d accede al monitor\n", id);

	// Attendo che il vettore non sia pieno
	while(p->count == DIM) {
		p->num_produttori_alta_prio++;
		pthread_cond_wait( &(p->not_full_prio1), &(p->mutex) );
		p->num_produttori_alta_prio--;
	}

	// Produzione

	p->buffer[p->testa] = value;
	p->testa = (p->testa + 1) % DIM;
	p->count++;

	printf("Produttore tipo 1 con id %d ha prodotto %d\n", id, value);


	// Riattivazione di un consumatore
	pthread_cond_signal( &(p->not_empty) );


	/* Uscita dal monitor */
	pthread_mutex_unlock( &(p->mutex) );

}

void produci_bassa_prio(PriorityProdCons* p, int id, int value){

	/* Ingresso nel monitor */
	pthread_mutex_lock( &(p->mutex) );

	printf("Produttore tipo 2 con id %d accede al monitor\n", id);


	//Attendo che il vettore NON sia pieno, e che NON vi siano altri produttori ad alta priorità in attesa

	/* NOTA BENE: a differenza dei produttori ad alta priorità, i
	 * produttori a bassa priorità devono assicurarsi che non siano già
	 * presenti dei produttori ad alta priorità in attesa, usando la
	 * condizione "p->num_produttori_alta_prio > 0".
	 *
	 * Questo controllo è importante nel seguente scenario:
	 *
	 *  - Si supponga che un thread consumatore abbia appena risvegliato
	 *    un thread produttore ad alta priorità.
	 *
	 *  - Poiché il monitor è di tipo signal-and-continue, il thread
	 *    consumatore non rilascia subito il monitor al momento della
	 *    signal, ed il thread produttore appena risvegliato si mette in
	 *    attesa sul mutex del monitor.
	 *
	 *  - Se nel frattempo sopraggiunge un thread produttore a bassa
	 *    priorità, si mette anche esso in attesa sul mutex del monitor.
	 *
	 *  - Quando il thread consumatore esce dal monitor, il thread
	 *    produttore a bassa priorità potrebbe entrare nel monitor PRIMA
	 *    del thread produttore ad alta priorità, poiché la mutex_lock
	 *    non fornisce alcuna garanzia sull'ordine di acquisizione.
	 *
	 *  - Con il controllo sul numero di produttori ad alta priorità in
	 *    attesa, i produttori a bassa priorità lasciano il posto ad un
	 *    eventuale produttore ad alta priorità in attesa.
	 */

	while(p->count == DIM || p->num_produttori_alta_prio > 0) {
		p->num_produttori_bassa_prio++;
		pthread_cond_wait( &(p->not_full_prio2), &(p->mutex) );
		p->num_produttori_bassa_prio--;
	}


	// Produzione

	p->buffer[p->testa] = value;
	p->testa = (p->testa + 1) % DIM;
	p->count++;

	printf("Produttore tipo 2 con id %d ha prodotto %d\n", id, value);


	// Riattivazione di un consumatore
	pthread_cond_signal( &(p->not_empty) );


	/* Uscita dal monitor */
	pthread_mutex_unlock( &(p->mutex) );

}

int consuma(PriorityProdCons* p, int id){

	int value;

	/* Ingresso nel monitor */
	pthread_mutex_lock( &(p->mutex) );

	printf("Consumatore con id %d accede al monitor\n", id);


	//Se non c'è nulla nel vettore, attendo che qualcuno produca
	while(p->count == 0){
		pthread_cond_wait( &(p->not_empty), &(p->mutex) );
	}


	// Consumazione

	value = p->buffer[p->coda];
	p->coda = (p->coda + 1) % DIM;
	p->count--;

	printf("Consumatore con id %d ha consumato %d\n", id, value);


	printf("Thread in attesa sulla condition NOT_FULL_1: %d\n", p->num_produttori_alta_prio);
	printf("Thread in attesa sulla condition NOT_FULL_2: %d\n", p->num_produttori_bassa_prio);

	// Si riattiva un produttore di tipo 1 se presente,
	// altrimenti si riattiva un produttore di tipo 2
	if(p->num_produttori_alta_prio > 0)
		pthread_cond_signal( &(p->not_full_prio1) );
	else
		pthread_cond_signal( &(p->not_full_prio2) );


	/* Uscita dal monitor */
	pthread_mutex_unlock( &(p->mutex) );


	return value;

}
