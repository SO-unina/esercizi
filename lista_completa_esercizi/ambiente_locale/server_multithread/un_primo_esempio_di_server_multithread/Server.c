#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "Header.h"

/* Il thread padre condivide l'id della coda
   delle risposte con i figli, tramite una
   variabile globale */

int id_coda_risposte;


/* Mutex per la mutua esclusione tra i worker */

pthread_mutex_t mutex;


void server(int id_c, int id_s){

	int k;
	int ret;

	id_coda_risposte = id_s;


	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_mutex_init(&mutex,NULL);

	while(1){

		msg_richiesta richiesta;

		ret = msgrcv(id_c, &richiesta, sizeof(msg_richiesta)-sizeof(long), 0, 0);

		if(ret < 0) {
			perror("Errore ricezione richiesta server");
			exit(1);
		}

		if(richiesta.v1==-1 && richiesta.v2==-1){
			exit(0);
		}


		/**********
		ATTENZIONE: è errato in questo contesto passare al worker
		un puntatore alla struttura dati del messaggio sullo stack,
		poiché lo spazio dello stack è privato per il thread e non deve
		essere condiviso con altri thread.

		Esempio di CODICE ERRATO:

		msg_richiesta richiesta;   // la variabile è sullo stack del thread principale
		msgrcv(id_c, &richiesta, sizeof(msg_richiesta)-sizeof(long), 0, 0);
		...
		pthread_create(..., &richiesta); // viene passato qui un puntatore verso lo stack
		...
		// mentre il worker esegue, il padre può modificare la variabile "richiesta"

		Per passare correttamente dei parametri ad un nuovo thread, è necessario creare una
		struttura dati in memoria heap (in questo caso, una copia del messaggio in memoria heap).

		**********/

		msg_richiesta * copia_messaggio = (msg_richiesta*) malloc(sizeof(msg_richiesta));

		copia_messaggio->v1 = richiesta.v1;
		copia_messaggio->v2 = richiesta.v2;
		copia_messaggio->pid = richiesta.pid;

		pthread_t t;
		pthread_create(&t, &attr, Prodotto, copia_messaggio);

	}

}



void* Prodotto(void* v){

	int ret;

	msg_richiesta * richiesta = (msg_richiesta *) v;

	msg_risposta risposta;

	risposta.pid = richiesta->pid;
	risposta.v3 = richiesta->v1 * richiesta->v2;

	pthread_mutex_lock(&mutex);

		printf("\nSono Prodotto di Server. Invio del calcolo: %d\n\n", risposta.v3);

		ret = msgsnd(id_coda_risposte, &risposta, sizeof(msg_risposta)-sizeof(long), 0);

		if(ret < 0) {
			perror("Errore invio risposta server");
			exit(1);
		}

	pthread_mutex_unlock(&mutex);

	free(richiesta);

	pthread_exit(NULL);
}

