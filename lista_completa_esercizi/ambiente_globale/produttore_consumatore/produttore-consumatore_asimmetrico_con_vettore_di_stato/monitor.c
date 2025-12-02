#include <stdio.h>
#include <unistd.h>

#include "header.h"


/* Funzioni di utilità */

int ricerca_libero(GestioneVoli * g);
int ricerca_volo(GestioneVoli * g, int identificativo);



void InserisciVolo(GestioneVoli * g, int identificativo) {

	pthread_mutex_lock( &(g->mutex) );

	while(g->num_liberi==0) {

		pthread_cond_wait( &(g->produttori), &(g->mutex) );
	}


	int id_buffer = ricerca_libero(g);

	g->vettore_stato[id_buffer] = IN_USO;

	g->num_liberi--;


	pthread_mutex_unlock( &(g->mutex) );



	sleep(1);

	printf("[debug] inserimento in [%d] del volo %d\n\n", id_buffer, identificativo);

	g->vettore_voli[id_buffer].identificativo = identificativo;
	g->vettore_voli[id_buffer].quota = 0;



	pthread_mutex_lock( &(g->mutex) );

	g->vettore_stato[id_buffer] = OCCUPATO;

	pthread_mutex_unlock( &(g->mutex) );


}




void RimuoviVolo(GestioneVoli * g, int identificativo) {

	pthread_mutex_lock( &(g->mutex) );


	int id_buffer = ricerca_volo(g, identificativo);

	if(id_buffer == -1) {

		printf("Errore: Volo %d non trovato\n", identificativo);

		pthread_mutex_unlock( &(g->mutex) );

		return;
	}


	g->vettore_stato[id_buffer] = IN_USO;


	pthread_mutex_unlock( &(g->mutex) );




	printf("[debug] rimozione in [%d] del volo %d\n\n", id_buffer, identificativo);

	sleep(1);




	pthread_mutex_lock( &(g->mutex) );


	g->num_liberi++;

	g->vettore_stato[id_buffer] = LIBERO;

	pthread_cond_signal( &(g->produttori) );


	pthread_mutex_unlock( &(g->mutex) );

}




void AggiornaVolo(GestioneVoli * g, int identificativo, int quota) {

	pthread_mutex_lock( &(g->mutex) );

	int id_buffer = ricerca_volo(g, identificativo);

	if(id_buffer == -1) {

		printf("Errore: Volo %d non trovato\n", identificativo);

		pthread_mutex_unlock( &(g->mutex) );

		return;
	}


	g->vettore_stato[id_buffer] = IN_USO;


	pthread_mutex_unlock( &(g->mutex) );




	printf("[debug] aggiornamento in [%d] del volo %d\n\n", id_buffer, identificativo);

	sleep(1);

	g->vettore_voli[id_buffer].quota = quota;




	pthread_mutex_lock( &(g->mutex) );

	g->vettore_stato[id_buffer] = OCCUPATO;

	pthread_mutex_unlock( &(g->mutex) );

}




int ricerca_libero(GestioneVoli * g) {

	int i = 0;


	while( g->vettore_stato[i] != LIBERO ) {

		i++;
	}


	return i;
}


int ricerca_volo(GestioneVoli * g, int identificativo) {

	int id_buffer = -1;

	int i=0;


	while( i<NUMERO_BUFFER && id_buffer==-1 ) {

		if(g->vettore_stato[i] == OCCUPATO &&
		   g->vettore_voli[i].identificativo == identificativo) {

			id_buffer = i;
		}

		i++;
	}


	return id_buffer;
}
