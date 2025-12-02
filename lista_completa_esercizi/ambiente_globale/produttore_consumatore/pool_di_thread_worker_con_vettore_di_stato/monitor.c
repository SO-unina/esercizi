#include <stdio.h>
#include <unistd.h>

#include "header.h"

int ricerca_libero(MonitorElaborazioni * m);
int ricerca_minimo(MonitorElaborazioni * m);



void inizializza(MonitorElaborazioni * m) {

	int i;

	for(i=0; i<NUMERO_BUFFER; i++) {

		m->stato[i] = LIBERO;
	}

	m->num_occupati = 0;
	m->num_liberi = NUMERO_BUFFER;

	pthread_mutex_init( &(m->mutex), NULL );

	pthread_cond_init( &(m->produttori), NULL );
	pthread_cond_init( &(m->consumatori), NULL );

}

void distruggi(MonitorElaborazioni * m) {

	pthread_mutex_destroy( &(m->mutex) );
	pthread_cond_destroy( &(m->produttori) );
	pthread_cond_destroy( &(m->consumatori) );

}

void produci_richiesta(MonitorElaborazioni * m, buffer * b) {

	pthread_mutex_lock( &(m->mutex) );


	while(m->num_liberi==0) {

		pthread_cond_wait( &(m->produttori), &(m->mutex) );
	}


	int id_buffer = ricerca_libero(m);

	m->stato[id_buffer] = IN_USO;

	m->num_liberi--;


	pthread_mutex_unlock( &(m->mutex) );




	printf("[debug] producendo in [%d]\n\n", id_buffer);

	m->elaborazioni[id_buffer].totale_operandi = b->totale_operandi;

	int i;
	for(i=0; i<b->totale_operandi; i++) {

		m->elaborazioni[id_buffer].operandi[i] = b->operandi[i];
	}




	pthread_mutex_lock( &(m->mutex) );


	m->num_occupati++;

	m->stato[id_buffer] = OCCUPATO;

	pthread_cond_signal( &(m->consumatori) );


	pthread_mutex_unlock( &(m->mutex) );
}




int consuma_richiesta(MonitorElaborazioni * m, buffer * b) {

	pthread_mutex_lock( &(m->mutex) );


	while(m->num_occupati==0) {

		pthread_cond_wait( &(m->consumatori), &(m->mutex) );
	}


	int id_buffer = ricerca_minimo(m);

	m->stato[id_buffer] = IN_USO;

	m->num_occupati--;


	pthread_mutex_unlock( &(m->mutex) );




	printf("[debug] consumando in [%d]\n\n", id_buffer);

	sleep( m->elaborazioni[id_buffer].totale_operandi );

	b->totale_operandi = m->elaborazioni[id_buffer].totale_operandi;

	int somma = 0;

	int i;
	for(i=0; i<b->totale_operandi; i++) {

		b->operandi[i] = m->elaborazioni[id_buffer].operandi[i];

		somma += b->operandi[i];
	}




	pthread_mutex_lock( &(m->mutex) );


	m->num_liberi++;

	m->stato[id_buffer] = LIBERO;

	pthread_cond_signal( &(m->produttori) );


	pthread_mutex_unlock( &(m->mutex) );


	return somma;
}



int ricerca_libero(MonitorElaborazioni * m) {

	int i = 0;


	while( m->stato[i] != LIBERO ) {

		i++;
	}


	return i;
}


int ricerca_minimo(MonitorElaborazioni * m) {

	int i = 0;

	int minimo = -1;
	int operandi_minimo = -1;


	for(i=0; i<NUMERO_BUFFER; i++) {

		if(m->stato[i] == OCCUPATO  &&  (minimo==-1  ||  m->elaborazioni[i].totale_operandi < operandi_minimo) ) {

			minimo = i;
			operandi_minimo = m->elaborazioni[i].totale_operandi;
		}
	}


	return minimo;
}


