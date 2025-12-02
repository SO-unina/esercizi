#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "prodcons.h"


void inizializza_prod_cons(PriorityProdCons* p){

	p->testa = 0;
	p->coda = 0;
    p->count = 0;

    p->threads_prio_1 = 0;
    p->threads_prio_2 = 0;

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

void produci_alta_prio(PriorityProdCons* p, int value){

    /* Ingresso nel monitor */

	pthread_mutex_lock(&(p->mutex));

	printf("Produttore tipo 1 accede al monitor\n");


    /* Controlla la condizione logica: il buffer NON deve essere pieno */

	while( p->count == DIM ) {

        p->threads_prio_1++;

        printf("Produttore tipo 1 in attesa\n");

		pthread_cond_wait(&(p->not_full_prio1), &(p->mutex));

        printf("Produttore tipo 1 risvegliato\n");

        p->threads_prio_1--;
    }



    /* Produzione */

	p->buffer[p->testa] = value;
	p->testa = (p->testa + 1) % DIM;
    p->count++;

	printf("Produttore tipo 1 ha prodotto %d\n", value);



    /* Sveglia un eventuale consumatore in attesa */

	pthread_cond_signal(&(p->not_empty));



    /* Uscita dal monitor */

    pthread_mutex_unlock(&(p->mutex));
}




void produci_bassa_prio(PriorityProdCons* p, int value){

    /* Ingresso nel monitor */

	pthread_mutex_lock(&(p->mutex));

	printf("Produttore tipo 2 accede al monitor\n");


    /* Controlla la condizione logica: il buffer NON deve essere pieno, e NON ci devono essere produttori ad alta priorità in attesa
     */

    /* NOTA BENE: a differenza dei produttori ad alta priorità, i produttori a bassa priorità devono assicurarsi che non siano già presenti dei produttori ad alta priorità ad attendere, usando la condizione "p->threads_prio_1 > 0".

       Questo controllo è importante nel seguente scenario:

        - Si supponga che un thread consumatore abbia appena risvegliato un thread produttore ad alta priorità.

        - Poiché il monitor è di tipo signal-and-continue, il thread consumatore non rilascia subito il monitor al momento della signal_condition, ed il thread produttore appena risvegliato si mette in attesa sul mutex del monitor.

        - Se nel frattempo sopraggiunge un thread produttore a bassa priorità, si mette anche esso in attesa sul mutex del monitor.

        - Quando il thread consumatore esce dal monitor, il thread produttore a bassa priorità potrebbe entrare nel monitor PRIMA del thread produttore ad alta priorità. Ciò può accadere poiché l'operazione di mutex_lock non fornisce alcuna garanzia sull'ordine con cui i thread acquisiranno il mutex. In altre parole, se due thread effettuano mutex_lock su un mutex bloccato, quando il lock viene sbloccato non è possibile prevedere quale dei due thread acquisirà il mutex.

        - Aggiungendo il controllo sul numero di produttori ad alta priorità in attesa, i thread produttori a bassa priorità lasciano il posto ad un eventuale thread produttore ad alta priorità in attesa.
     */

	while( p->count == DIM || p->threads_prio_1 > 0 ) {

        p->threads_prio_2++;

        printf("Produttore tipo 2 in attesa\n");

		pthread_cond_wait(&(p->not_full_prio2), &(p->mutex));

        printf("Produttore tipo 2 risvegliato\n");

        p->threads_prio_2--;
	}



    /* Produzione */

	p->buffer[p->testa] = value;
	p->testa = (p->testa + 1) % DIM;
    p->count++;

	printf("Produttore tipo 2 ha prodotto %d\n", value);



    /* Sveglia un eventuale consumatore in attesa */

	pthread_cond_signal(&(p->not_empty));



    /* Uscita dal monitor */

    pthread_mutex_unlock(&(p->mutex));

}

int consuma(PriorityProdCons* p){

	int value;

    /* Ingresso nel monitor */

	pthread_mutex_lock(&(p->mutex));

	printf("Consumatore accede al monitor\n");


    /* Controlla la condizione logica: il buffer NON deve essere vuoto */

	while( p->count == 0 ) {

        printf("Consumatore in attesa\n");

		pthread_cond_wait(&(p->not_empty), &(p->mutex));

        printf("Consumatore risvegliato\n");
    }



    /* Consumazione */

	value = p->buffer[p->coda];
	p->coda = (p->coda + 1) % DIM;
	p->count--;

	printf("Consumatore ha consumato %d\n", value);



    /* Se presenti, sveglia un thread produttore ad alta priorità,
       altrimenti sveglia un thread produttore a bassa priorità */

	if(p->threads_prio_1 > 0) {

        printf("Consumatore risveglia produttore ad alta priorità\n");

		pthread_cond_signal(&(p->not_full_prio1));

	} else if(p->threads_prio_2 > 0) {

        printf("Consumatore risveglia produttore a bassa priorità\n");

        pthread_cond_signal(&(p->not_full_prio2));
    }


    /* Uscita dal monitor */

	pthread_mutex_unlock(&(p->mutex));


    return value;

}
