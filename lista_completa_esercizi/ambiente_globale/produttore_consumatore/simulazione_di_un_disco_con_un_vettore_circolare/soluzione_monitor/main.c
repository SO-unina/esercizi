#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "header.h"


void * Utente(void * arg) {

	ArgomentiThread * a = (ArgomentiThread *) arg;

	int i;

	for(i=0; i<TOTALE_RICHIESTE; i++) {

		/*
		  Creazione e inserimento di una richiesta
		*/

		richiesta r;

		r.posizione = rand() % TOTALE_POSIZIONI;
		r.id_utente = a->id;

		printf("[Utente %d] Richiesta Utente: posizione=%u, id_utente=%d\n", a->id, r.posizione, r.id_utente);

		InserisciRichiesta(a->s, a->id, &r);
	}

	pthread_exit(NULL);
}


void * Schedulatore(void * arg) {

	ArgomentiThread * a = (ArgomentiThread *) arg;

	int i;

	/*
	  Variabile locale contenente la posizione dell'ultima richiesta
	  che è stata servita (inizialmente si assume la posizione 0)
	*/

	int posizione_corrente = 0;



	/*
	  Array locale che rappresenta il disco. Ogni locazione del
	  disco ospiterà l'identificativo del thread Utente che ha
	  richiesto l'ultima operazione in quella posizione
	*/

	int disco[TOTALE_POSIZIONI];



	for(i=0; i<TOTALE_RICHIESTE*TOTALE_UTENTI; i++) {

		richiesta r;



		/*
		  Lo Schedulatore preleva una richiesta, che verrà copiata
		  nella variabile locale "r"
		*/

		printf("Schedulatore in attesa di richieste...\n");

		PrelevaRichiesta(a->s, &r);

		printf("[Schedulatore] Prelevo richiesta: posizione=%u, id_utente=%d\n", r.posizione, r.id_utente);


		/*
		  Lo Schedulatore attende alcuni decimi di secondo (in base alla
		  posizione della richiesta), ed aggiorna la posizione del disco
		  con l'identificativo del thread richiedente
		*/

		int attesa = abs(posizione_corrente - (int)r.posizione);

		printf("[Schedulatore] Attesa Schedulatore... (%d decimi di secondo)\n", attesa);

		usleep(attesa * 100000);



		posizione_corrente = r.posizione;

		disco[posizione_corrente] = r.id_utente;



		printf("Disco aggiornato alla posizione %u, nuovo valore %d\n", r.posizione, disco[posizione_corrente]);

	}

	pthread_exit(NULL);
}


int main() {

	int i;

	pthread_t thread_utenti[TOTALE_UTENTI];
	pthread_t thread_schedulatore;

	ArgomentiThread argomenti[TOTALE_UTENTI];
	ArgomentiThread argomenti_schedulatore;


	/*
	  La coda di richieste non richiede più una memoria condivisa:
	  è una normale struttura allocata nel main, condivisa da tutti
	  i thread attraverso il puntatore passato come argomento
	*/

	MonitorSchedulatore * s = malloc(sizeof(MonitorSchedulatore));

	if( s == NULL ) {

		perror("Errore malloc()");
		exit(1);
	}



	InizializzaMonitor(s);

	srand(time(NULL));


	/*
	  Creazione dei thread Utente, ricevono in ingresso il proprio
	  identificativo ed un puntatore al monitor
	*/

	for(i=0; i<TOTALE_UTENTI; i++) {

		argomenti[i].id = i+1;
		argomenti[i].s = s;

		printf("Thread utente %d in esecuzione\n", argomenti[i].id);

		pthread_create(&thread_utenti[i], NULL, Utente, (void *) &argomenti[i]);
	}



	/*
	  Creazione del thread Schedulatore, riceve in ingresso
	  un puntatore al monitor
	*/

	argomenti_schedulatore.id = 0;
	argomenti_schedulatore.s = s;

	printf("Thread schedulatore in esecuzione\n");

	pthread_create(&thread_schedulatore, NULL, Schedulatore, (void *) &argomenti_schedulatore);



	/*
	  Il thread main si pone in attesa della terminazione
	  dei thread Utente e del thread Schedulatore
	*/

	for(i=0; i<TOTALE_UTENTI; i++) {

		pthread_join(thread_utenti[i], NULL);

		printf("Thread utente terminato\n");
	}

	pthread_join(thread_schedulatore, NULL);

	printf("Thread schedulatore terminato\n");



	/*
	  Deallocazione delle risorse
	*/

	RimuoviMonitor(s);

	free(s);


	return 0;
}
