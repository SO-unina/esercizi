#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

#include "header.h"

int main() {

	mqd_t msg_id_balancer;
	mqd_t msg_id_server[TOTALE_SERVER];
	char nome_server[TOTALE_SERVER][QUEUE_NAME_SIZE];
	int i;
	pid_t p;
	int status;
	int ret;


	/* Attributi espliciti delle code: capacita' e dimensione del
	   singolo messaggio */

	struct mq_attr attr = {0};
	attr.mq_maxmsg = MAX_MESSAGGI_CODA;
	attr.mq_msgsize = sizeof(struct messaggio);


	/*
	  Creazione di una unica coda per i messaggi dai client
	  verso il balancer. mq_unlink() preventivo per rimuovere
	  eventuali residui di esecuzioni precedenti.
	*/

	mq_unlink(QUEUE_BALANCER);

	msg_id_balancer = mq_open(QUEUE_BALANCER, O_CREAT | O_RDWR, 0664, &attr);

	if(msg_id_balancer == (mqd_t)-1) {

		perror("Errore mq_open() coda balancer");
		exit(1);
	}



	/*
	  Creazione di 3 code per i messaggi dal balancer verso
	  i server, una coda per ogni server
	*/

	for(i=0; i<TOTALE_SERVER; i++) {

		sprintf(nome_server[i], QUEUE_SERVER_PREFIX "%d", i+1);

		mq_unlink(nome_server[i]);

		msg_id_server[i] = mq_open(nome_server[i], O_CREAT | O_RDWR, 0664, &attr);

		if(msg_id_server[i] == (mqd_t)-1) {

			perror("Errore mq_open() coda server");
			exit(1);
		}
	}



	/*
	  Creazione processi client, ricevono in ingresso il
	  descrittore della coda del balancer (i descrittori mqd_t
	  sono ereditati attraverso la fork)
	*/

	for(i=0; i<TOTALE_CLIENT; i++) {


		p = fork();

		if(p==0) {

			printf("Processo figlio client in esecuzione (PID %d)\n", getpid());

			Client(msg_id_balancer);

			exit(0);
		}

		if(p<0) {
			perror("Errore fork() client");
			exit(1);
		}
	}



	/*
	  Creazione processi server, ricevono in ingresso il
	  descrittore della propria coda
	*/

	for(i=0; i<TOTALE_SERVER; i++) {


		p = fork();

		if(p==0) {

			printf("Processo figlio server in esecuzione (PID %d)\n", getpid());

			Server(msg_id_server[i]);

			exit(0);
		}

		if(p<0) {
			perror("Errore fork() server");
			exit(1);
		}
	}



	/*
	  Creazione del processo balancer, riceve in ingresso
	  i descrittori di tutte le code
	*/


	p = fork();

	if(p==0) {

		printf("Processo figlio balancer in esecuzione (PID %d)\n", getpid());

		Balancer(msg_id_balancer, msg_id_server);

		exit(0);
	}

	if(p<0) {
		perror("Errore fork() balancer");
		exit(1);
	}



	/*
	  Il processo padre si pone in attesa della terminazione
	  di tutti i processi (client, server e balancer)
	*/

	for(i=0; i<TOTALE_CLIENT+TOTALE_SERVER+1; i++) {

		p = wait( &status );

		if(p<0) {

			perror("Errore wait()");
			exit(1);
		}


		/*
		  Si verifica lo stato di terminazione dei processi figli.
		  Se il figlio è uscito mediante una exit(0), allora non si
		  sono verificati errori. In caso di errori, i figli segnalano
		  al padre l'anomalia uscendo con exit(1).
		*/

		if( WIFEXITED(status) && WEXITSTATUS(status) == 0 ) {

			printf("Il processo %d è terminato correttamente\n", p);

		} else {

			printf("Il processo %d è terminato in modo anomalo\n", p);
		}
	}



	/*
	  Rimozione delle code dal sistema: mq_close() chiude il
	  descrittore, mq_unlink() rimuove il nome. Il nome sparisce
	  subito, la coda viene distrutta quando tutti i processi che
	  la avevano aperta la chiudono.
	*/

	printf("Rimozione code di messaggi\n");

	mq_close(msg_id_balancer);

	ret = mq_unlink(QUEUE_BALANCER);

	if(ret < 0) {
		perror("Errore mq_unlink() balancer");
		exit(1);
	}


	for(i=0; i<TOTALE_SERVER; i++) {

		mq_close(msg_id_server[i]);

		ret = mq_unlink(nome_server[i]);

		if(ret < 0) {
			perror("Errore mq_unlink() server");
			exit(1);
		}
	}


	return 0;

}
