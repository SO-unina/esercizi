#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

void Sender(mqd_t id_queue_receiver, mqd_t id_queue_sender){

	struct mesg mess; // struct del messaggio
	int ret;

	while(1){

		// Prelevo il messaggio da inviare
		printf("Inserisci il messaggio da inviare [max 19 caratteri]\n");

		if(scanf("%19s", mess.message) != 1) {
			/* Fine dell'input (EOF): equivale ad un "exit" */
			strcpy(mess.message, "exit");
		}

		// Verifico se il messaggio è exit
		if(strcmp(mess.message,"exit")==0){

			/* Essendo exit, lo invio sulla seconda coda (per far
			 * terminare il Receiver locale) e termino. L'intero
			 * messaggio viene inviato: non c'e' piu' il campo mtype
			 * da scorporare dalla dimensione. */
			ret = mq_send(id_queue_receiver, (const char *)&mess, sizeof(mess), 0);
			if(ret<0) {
				//perror("mq_send fallita");
				exit(1);
			}

			printf("[SENDER] inviato: %s\n", mess.message);

			exit(0);

		}else{

			// Non essendo exit, lo invio sulla prima coda
			ret = mq_send(id_queue_sender, (const char *)&mess, sizeof(mess), 0);

			if(ret<0) {
				//perror("mq_send fallita");
				exit(1);
			}

			printf("[SENDER] inviato: %s\n",mess.message);
		}
	}

}

void Receiver(mqd_t id_queue_receiver){

	struct mesg mess; // struct del messaggio
	ssize_t ret;

	while(1){

		/* Mi metto in attesa di un messaggio sulla seconda coda.
		 * Il buffer deve essere grande esattamente mq_msgsize
		 * (sizeof(struct mesg)): con un buffer piu' piccolo
		 * mq_receive fallisce con EMSGSIZE. */
		ret = mq_receive(id_queue_receiver, (char *)&mess, sizeof(mess), NULL);

		if(ret<0) {
			//perror("mq_receive fallita");
			exit(1);
		}

		printf("[RECEIVER] ricevuto: %s\n",mess.message);

		// Se il messaggio ricevuto è exit termino
		if(strcmp(mess.message,"exit")==0){
			exit(0);
		}

	}

}
