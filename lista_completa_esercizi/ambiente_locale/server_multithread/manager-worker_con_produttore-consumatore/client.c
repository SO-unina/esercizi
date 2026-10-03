#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "header.h"

#define NUM_REQ 5


int main(){

	/* Apre la coda delle richieste creata dal programma principale */
	mqd_t id_send = mq_open(CODA_RICHIESTE, O_WRONLY);

	if(id_send == (mqd_t)-1){
		perror("[Client] - Errore mq_open richieste");
		exit(1);
	}

	/* Crea la propria coda di risposta, il cui nome contiene il PID:
	 * sostituisce la ricezione selettiva con mtype=PID. */
	char nome_coda[NOME_CODA_MAX];
	sprintf(nome_coda,CODA_RISPOSTA_FMT,getpid());

	struct mq_attr attr = {0};
	attr.mq_maxmsg = MAX_MESSAGGI;
	attr.mq_msgsize = sizeof(Risposta);

	mq_unlink(nome_coda);

	mqd_t id_res = mq_open(nome_coda, O_CREAT | O_RDONLY, 0664, &attr);

	if(id_res == (mqd_t)-1){
		perror("[Client] - Errore mq_open risposta");
		exit(1);
	}

	printf("[Client %d] - invio richieste...\n",getpid());


	int i = 0;
	Messaggio mes;
	Risposta res;

	srand(time(NULL));

	while(i < NUM_REQ){

		mes.pid = getpid();
		mes.op1 = rand()%10;
		mes.op2 = rand()%10;

		// Invio di una richiesta
		printf("[Client %d] - Invio richiesta {%d,%d} al server...\n",getpid(),mes.op1,mes.op2);
		mq_send(id_send,(const char *)&mes,sizeof(Messaggio),0);

		// In attesa della risposta sulla propria coda
		mq_receive(id_res,(char *)&res,sizeof(Risposta),NULL);
		printf("[Client %d] - Ho ricevuto il risultato %d\n",getpid(),res.risultato);

		i++;
	}

	/* Chiusura e rimozione della propria coda di risposta */
	mq_close(id_send);
	mq_close(id_res);
	mq_unlink(nome_coda);

	printf("[Client %d] - Fine processo\n",getpid());

	return 0;
}
