#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/types.h>
#include <unistd.h>

#include "Header.h"

void client(mqd_t coda_richieste){

	int k;
	int ret;
	ssize_t nrecv;

	srand(getpid());

	msg_richiesta richiesta;
	msg_risposta risposta;

	richiesta.pid = getpid();
	richiesta.v1 = rand()%101;
	richiesta.v2 = rand()%101;

	/* Ogni client crea una propria coda di risposta, il cui nome
	 * contiene il PID: sostituisce la ricezione selettiva per tipo
	 * (mtype=PID) delle code System V. */
	char nome_coda[NOME_CODA_MAX];
	sprintf(nome_coda, CODA_RISPOSTA_FMT, getpid());

	struct mq_attr attr = {0};
	attr.mq_maxmsg = MAX_MESSAGGI;
	attr.mq_msgsize = sizeof(msg_risposta);

	mq_unlink(nome_coda);

	mqd_t coda_risposta = mq_open(nome_coda, O_CREAT | O_RDONLY, 0664, &attr);

	if(coda_risposta == (mqd_t)-1) {
		perror("Errore creazione coda risposta client");
		exit(1);
	}

	for(k=0;k<RICHIESTE;k++){

		printf("Richiesta %d Inviata (%d, %d) [PID=%d]\n\n", k, richiesta.v1, richiesta.v2, richiesta.pid);

		ret = mq_send(coda_richieste, (const char *)&richiesta, sizeof(msg_richiesta), 0);

		if(ret < 0) {
			perror("Errore invio richiesta client");
			exit(1);
		}



		nrecv = mq_receive(coda_risposta, (char *)&risposta, sizeof(msg_risposta), NULL);

		if(nrecv < 0) {
			perror("Errore ricezione risposta client");
			exit(1);
		}

		printf("Risposta %d Ricevuta (%d) [PID=%d]\n\n", k, risposta.v3, risposta.pid);
	}

	/* Chiusura e rimozione della coda di risposta del client */
	mq_close(coda_risposta);
	mq_unlink(nome_coda);
	mq_close(coda_richieste);

}
