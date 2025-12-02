#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "header.h"



// PRIMA SOLUZIONE AL PASSAGGIO DI PARAMETRI:
// Passaggio tramite una struttura dati
// (sia per l'istanza di monitor, sia per l'identificativo)

typedef struct {
    int identificativo;
    GestioneVoli * g;
} parametri;




// SECONDA SOLUZIONE AL PASSAGGIO DI PARAMETRI:
// Uso di una variabile globale
// (per l'istanza di monitor)

//GestioneVoli * g;





void * thread_gestori(void * p) {

	// PRIMA SOLUZIONE AL PASSAGGIO DI PARAMETRI:
	// Passaggio tramite una struttura dati

	parametri * param = (parametri *)p;
	int identificativo = param->identificativo;
	GestioneVoli * g = param->g;



	// SECONDA SOLUZIONE:
	// Cast del puntatore ad un intero
	// (per l'identificativo del thread)

	//int identificativo = (int) (long)p;




	printf("[THREAD %d] Inserimento\n\n", identificativo);

	InserisciVolo(g, identificativo);


	printf("[THREAD %d] Aggiornamento (1000)\n\n", identificativo);

	AggiornaVolo(g, identificativo, 1000);


	printf("[THREAD %d] Aggiornamento (2000)\n\n", identificativo);

	AggiornaVolo(g, identificativo, 2000);


	printf("[THREAD %d] Aggiornamento (1000)\n\n", identificativo);

	AggiornaVolo(g, identificativo, 1000);


	printf("[THREAD %d] Rimozione\n\n", identificativo);

	RimuoviVolo(g, identificativo);


	pthread_exit(0);
}



int main() {


	pthread_t threads[5];

	pthread_attr_t attr;

	int i, ret;

	// PRIMA SOLUZIONE:
	// Passaggio di parametri tramite una struttura
	GestioneVoli * g = (GestioneVoli *)malloc(sizeof(GestioneVoli));

	// SECONDA SOLUZIONE:
	// Uso di una variabile globale 
	// (per l'istanza di monitor)
	//g = (GestioneVoli *)malloc(sizeof(GestioneVoli));

	pthread_mutex_init( &(g->mutex), NULL );
	pthread_cond_init( &(g->produttori), NULL );

	g->num_liberi = NUMERO_BUFFER;

	for(i=0; i<NUMERO_BUFFER; i++) {

		g->vettore_stato[i] = LIBERO;
	}



	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    
	// PRIMA SOLUZIONE:
	// Passaggio di parametri tramite una struttura
	parametri param[5];


	for(i=0; i<5; i++) {

		// PRIMA SOLUZIONE:
		// Passaggio di parametri tramite una struttura

		param[i].identificativo = i;
		param[i].g = g;
		ret = pthread_create( &threads[i], &attr, thread_gestori, &param[i]);
        


		// SECONDA SOLUZIONE:
		// Cast da intero a puntatore
		// (per l'identificativo del thread)
		
		//ret = pthread_create( &threads[i], &attr, thread_gestori, (void*) (long)i);


        
		if(ret) {
			printf("Errore creazione thread (%d)\n", ret);
			exit(-1);
		}

	}



	for(i=0; i<5; i++) {

		ret = pthread_join( threads[i], NULL );

		if(ret) {
			printf("Errore join thread (%d)\n", ret);
			exit(-1);
		}
	}



	free(g);

}

