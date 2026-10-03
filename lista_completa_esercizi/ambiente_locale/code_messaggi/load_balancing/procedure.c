#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <mqueue.h>

#include "header.h"


void Client(mqd_t msg_id_balancer) {

	int i;
	int ret;

	for(i=0; i<TOTALE_MESSAGGI; i++) {

		struct messaggio m;

		m.PID = getpid();

		printf("Client %d: invio messaggio numero %d\n", getpid(), i);

		/* L'intero messaggio viene inviato: non c'e' piu' il campo
		   mtype da scorporare dalla dimensione. La priorita' (ultimo
		   parametro) non e' utilizzata in questo esercizio. */

		ret = mq_send(msg_id_balancer, (const char *)&m, sizeof(struct messaggio), 0);

		if(ret < 0) {

			perror("Errore mq_send() client");
			exit(1);
		}

		sleep(1);
	}
}


void Balancer(mqd_t msg_id_balancer, mqd_t msg_id_server[]) {

	int i;
	int ret;


	/*
	  La variabile "server" indica il prossimo server a cui
	  verrà inviato un messaggio. Viene utilizzata per accedere
	  all'array msg_id_server con i descrittori delle code dei server.
	*/

	int server = 0;


	for(i=0; i<TOTALE_MESSAGGI*TOTALE_CLIENT; i++) {

		struct messaggio m;


		/* Il buffer di ricezione deve essere grande esattamente
		   mq_msgsize: con un buffer piu' piccolo mq_receive
		   fallisce con EMSGSIZE. */

		ret = mq_receive(msg_id_balancer, (char *)&m, sizeof(struct messaggio), NULL);

		if(ret < 0) {

			perror("Errore mq_receive() balancer");
			exit(1);
		}



		printf("Balancer: ricezione messaggio dal processo %d, invio al server %d\n", m.PID, server+1);



		ret = mq_send(msg_id_server[server], (const char *)&m, sizeof(struct messaggio), 0);

		if(ret < 0) {

			perror("Errore mq_send() balancer");
			exit(1);
		}



		/*
		  Aggiorna la variabile server con la divisione in modulo
		*/

		server = (server + 1) % TOTALE_SERVER;
	}

}


void Server(mqd_t msg_id_server) {

	int i;
	int ret;


	/*
	  Ciascun server effettua un numero di ricezioni prefissato
	  e poi termina.

	  Nota bene: il codice assume che il numero totale di
	  messaggi sia esattamente un multiplo del numero di server
	*/

	for(i=0; i<TOTALE_MESSAGGI*TOTALE_CLIENT/TOTALE_SERVER; i++) {


		struct messaggio m;

		ret = mq_receive(msg_id_server, (char *)&m, sizeof(struct messaggio), NULL);

		if(ret < 0) {

			perror("Errore mq_receive() server");
			exit(1);
		}

		printf("Server %d: ricezione messaggio numero %d dal processo %d\n", getpid(), i, m.PID);
	}
}
