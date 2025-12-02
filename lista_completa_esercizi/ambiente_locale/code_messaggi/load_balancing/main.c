#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

int main() {

	int msg_id_balancer;
	int msg_id_server[TOTALE_SERVER];
	int i;
	pid_t p;
	int status;
	int ret;



	/*
	  Creazione di una unica coda per i messaggi dai client
	  verso il balancer
	*/

	msg_id_balancer = msgget(IPC_PRIVATE, IPC_CREAT|0664);

	if(msg_id_balancer < 0) {

		perror("Errore msgget() coda balancer");
		exit(1);
	}



	/*
	  Creazione di 3 code per i messaggi dal balancer verso
	  i server, una coda per ogni server
	*/

	for(i=0; i<TOTALE_SERVER; i++) {

		msg_id_server[i] = msgget(IPC_PRIVATE, IPC_CREAT|0664);

		if(msg_id_server[i] < 0) {

			perror("Errore msgget() coda server");
			exit(1);
		}
	}



	/*
	  Creazione processi client, ricevono in ingresso l'ID
	  della coda del balancer
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
	  Creazione processi server, ricevono in ingresso l'ID
	  della propria coda
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
	  gli ID di tutte le code
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
	  Rimozione delle code dal sistema
	*/

	printf("Rimozione code di messaggi\n");

	ret = msgctl(msg_id_balancer, IPC_RMID, NULL);

	if(ret < 0) {
		perror("Errore msgctl() balancer");
		exit(1);
	}


	for(i=0; i<TOTALE_SERVER; i++) {

		ret = msgctl(msg_id_server[i], IPC_RMID, NULL);

		if(ret < 0) {
			perror("Errore msgctl() server");
			exit(1);
		}
	}


	return 0;

}


