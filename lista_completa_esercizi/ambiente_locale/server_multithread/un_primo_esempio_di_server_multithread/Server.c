#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/types.h>
#include <unistd.h>

#include "Header.h"


/* Mutex per la mutua esclusione tra i worker
   nell'invio delle risposte */

pthread_mutex_t mutex;


void server(mqd_t coda_richieste){

	ssize_t ret;

	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_mutex_init(&mutex,NULL);

	while(1){

		msg_richiesta richiesta;

		/* Il buffer di ricezione deve essere grande almeno quanto
		 * mq_msgsize della coda, altrimenti mq_receive fallisce
		 * con EMSGSIZE. */
		ret = mq_receive(coda_richieste, (char *)&richiesta, sizeof(msg_richiesta), NULL);

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
		mq_receive(coda_richieste, (char*)&richiesta, sizeof(msg_richiesta), NULL);
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

	/* La risposta viene inviata sulla coda dedicata al client,
	 * il cui nome contiene il PID (al posto della ricezione
	 * selettiva con mtype=PID delle code System V). */
	char nome_coda[NOME_CODA_MAX];
	sprintf(nome_coda, CODA_RISPOSTA_FMT, richiesta->pid);

	pthread_mutex_lock(&mutex);

		printf("\nSono Prodotto di Server. Invio del calcolo: %d\n\n", risposta.v3);

		mqd_t coda_risposta = mq_open(nome_coda, O_WRONLY);

		if(coda_risposta == (mqd_t)-1) {
			perror("Errore apertura coda risposta server");
			exit(1);
		}

		ret = mq_send(coda_risposta, (const char *)&risposta, sizeof(msg_risposta), 0);

		if(ret < 0) {
			perror("Errore invio risposta server");
			exit(1);
		}

		mq_close(coda_risposta);

	pthread_mutex_unlock(&mutex);

	free(richiesta);

	pthread_exit(NULL);
}
