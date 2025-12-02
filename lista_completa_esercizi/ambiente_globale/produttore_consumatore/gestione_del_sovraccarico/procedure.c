#include <stdio.h>
#include <unistd.h>

#include "header.h"

void inizializza(MonitorCoda *m) {

	pthread_mutex_init( &(m->mutex), NULL );
	pthread_cond_init( &(m->cv_produttori), NULL );
	pthread_cond_init( &(m->cv_consumatori), NULL );

	m->testa = 0;
	m->coda = 0;
	m->numero_elementi = 0;
	m->produttori_in_attesa = 0;

}


// il valore di ritorno di produzione() indica se il monitor è sovraccarico
int produzione(MonitorCoda *m, int valore) {

	pthread_mutex_lock( &(m->mutex) );

	if( m->produttori_in_attesa >= 3 ) {

		pthread_mutex_unlock( &(m->mutex) );
		return 1;
	}


	while( m->numero_elementi == DIMENSIONE ) {

		m->produttori_in_attesa++;

		printf("PRODUTTORI IN ATTESA: %d\n", m->produttori_in_attesa);

		pthread_cond_wait( &(m->cv_produttori), &(m->mutex) );

		m->produttori_in_attesa--;
	}


	printf("IN PRODUZIONE...\n");

	m->vettore[ m->testa ] = valore;
	m->testa = (m->testa + 1) % DIMENSIONE;

	m->numero_elementi++;


	pthread_cond_signal( &(m->cv_consumatori) );

	pthread_mutex_unlock( &(m->mutex) );


	return 0;
}


// il valore di ritorno di consumazione() è il valore prelevato dalla coda
int consumazione(MonitorCoda *m) {

	int valore;


	pthread_mutex_lock( &(m->mutex) );

	while( m->numero_elementi == 0 ) {

		pthread_cond_wait( &(m->cv_consumatori), &(m->mutex) );
	}


	printf("IN CONSUMAZIONE...\n");

	valore = m->vettore[ m->coda ];
	m->coda = (m->coda + 1) % DIMENSIONE;

	m->numero_elementi--;


	pthread_cond_signal( &(m->cv_produttori) );

	pthread_mutex_unlock( &(m->mutex) );


	return valore;
}


