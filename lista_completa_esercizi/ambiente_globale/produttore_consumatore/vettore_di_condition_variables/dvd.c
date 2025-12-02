#include "dvd.h"

#include <stdio.h>



void inizializza(struct Monitor * m) {

	pthread_mutex_init( &(m->mutex), NULL );

	pthread_cond_init( &(m->cv[0]), NULL );
	pthread_cond_init( &(m->cv[1]), NULL );
	pthread_cond_init( &(m->cv[2]), NULL );

	m->dvd[0].identificativo_film = 1;
	m->dvd[0].identificativo_copia = 1;
	m->dvd[0].stato = DISPONIBILE;

	m->dvd[1].identificativo_film = 1;
	m->dvd[1].identificativo_copia = 2;
	m->dvd[1].stato = DISPONIBILE;

	m->dvd[2].identificativo_film = 2;
	m->dvd[2].identificativo_copia = 1;
	m->dvd[2].stato = DISPONIBILE;

	m->dvd[3].identificativo_film = 2;
	m->dvd[3].identificativo_copia = 2;
	m->dvd[3].stato = DISPONIBILE;

	m->dvd[4].identificativo_film = 3;
	m->dvd[4].identificativo_copia = 1;
	m->dvd[4].stato = DISPONIBILE;

	m->dvd[5].identificativo_film = 3;
	m->dvd[5].identificativo_copia = 2;
	m->dvd[5].stato = DISPONIBILE;

}

void distruggi(struct Monitor * m) {

	pthread_mutex_destroy( &(m->mutex) );

	pthread_cond_destroy( &(m->cv[0]) );
	pthread_cond_destroy( &(m->cv[1]) );
	pthread_cond_destroy( &(m->cv[2]) );
}





int affitta(struct Monitor * m, int id_film) {

	/*
	 * Controllo sulla correttezza del parametro id_film
	 *
	 * NB: il controllo dei parametri è facoltativo negli esercizi
	 *     del corso, ma indispensabile nello sviluppo di monitor
	 *     o di altre strutture dati usate in progetti complessi
	 */

	if(id_film < 0 || id_film >= NUM_DVD) {

		return -1;
	}



	pthread_mutex_lock( &(m->mutex) );

	printf("[AFFITTA] Richiesta di affitto film %d\n", id_film);



	/*
	 * Il thread attende qui fin quando non trova un film "id_film" in stato "DISPONIBILE".
	 * La ricerca ha esito positivo quando la funzione "ricerca_dvd" restituisce un valore
	 * maggiore o uguale a 0, da usare come indice nel vettore di DVD.
	 * Essa ritorna -1 nel caso non vi siano DVD disponibili.
	 *
	 */

	int i = ricerca_dvd(m->dvd, id_film);

	while( i == -1 ) {

		/* I thread che richiedono il film i-esimo devono attentere sulla CV "i-1"
		 * (ad esempio, I thread che richiedono il film 1 devono attentere sulla CV 0)
		 */

		pthread_cond_wait( &(m->cv[id_film-1]), &(m->mutex) );

		i = ricerca_dvd(m->dvd, id_film);
	}



	m->dvd[i].stato = AFFITTATO;

	int id_copia = m->dvd[i].identificativo_copia;

	printf("[AFFITTA] Affittato film %d, copia %d\n", id_film, id_copia);



	pthread_mutex_unlock( &(m->mutex) );

	return id_copia;
}




/*
 * La funzione "ricerca_dvd" cerca un DVD disponibile all'interno di "vettore".
 *
 * Restituisce la posizione del DVD nel vettore se presente, altrimenti il valore -1.
 *
 * NB: Questa funzione non fa parte dei metodi del monitor invocabili dai processi, ma è
 *     solo una funzione di supporto. Se si trattasse di una classe, sarebbe un metodo privato.
 *
 */
int ricerca_dvd(struct DVD vettore [], int id_film) {

	int i = 0;

	while(i<NUM_DVD) {

		if( vettore[i].identificativo_film==id_film && vettore[i].stato==DISPONIBILE ) {

			return i;
		}

		i++;
	}

	return -1;
}




void restituisci(struct Monitor * m, int id_film, int id_copia) {

	pthread_mutex_lock( &(m->mutex) );


	int i = 0;
	int found = 0;


	/*
	 * Si scorre il vettore fino a quando non viene trovato un DVD
	 * con il codice film ed il codice copia da restituire
	 *
	 */

	while(i<NUM_DVD && !found) {

		if( m->dvd[i].identificativo_film==id_film && m->dvd[i].identificativo_copia==id_copia ) {

			found = 1;

		} else {

			i++;
		}
	}


	/*
	 * Forza l'uscita dal monitor nel caso di DVD non trovato
	 * (è un controllo facoltativo sui parametri)
	 *
	 */

	if(!found) {

		pthread_mutex_unlock( &(m->mutex) );
		return;
	}




	/* Rende disponibile il DVD */

	m->dvd[i].stato = DISPONIBILE;

	printf("[RESTITUISCI] Restituzione film %d, copia %d\n", id_film, id_copia);




	/* Riattiva un eventuale thread in attesa del film restituito */

	pthread_cond_signal( &(m->cv[id_film-1]) );


	pthread_mutex_unlock( &(m->mutex) );
}



void stampa(struct Monitor * m) {

	pthread_mutex_lock( &(m->mutex) );


	printf("[STAMPA]");

	int i;

	for(i=0; i<NUM_DVD; i++) {

		if(m->dvd[i].stato == DISPONIBILE) {

			printf(" (film %d, copia %d, DISP.)", m->dvd[i].identificativo_film, m->dvd[i].identificativo_copia);

		} else {

			printf(" (film %d, copia %d, AFF.)", m->dvd[i].identificativo_film, m->dvd[i].identificativo_copia);

		}
	}

	printf("\n");

	pthread_mutex_unlock( &(m->mutex) );
}


