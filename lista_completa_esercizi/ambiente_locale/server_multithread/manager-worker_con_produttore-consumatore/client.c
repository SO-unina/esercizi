#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "header.h"

#define NUM_REQ 5


int main(){
        int id_send,id_res;
        pid_t pid;

        key_t key_send,key_res;
        key_send = ftok(FTOK_QUEUE_CMD_PATH,FTOK_QUEUE_CMD_TOK);
        key_res = ftok(FTOK_QUEUE_RES_PATH,FTOK_QUEUE_RES_TOK);

        id_send = msgget(key_send,0);
        id_res = msgget(key_res,0);

	printf("[DEBUG] - client id1 %d, id2 %d\n",id_send,id_res);	
	printf("[Client %d] - invio richieste...\n",getpid());


	int i = 0;
	Messaggio mes;	
	Risposta res;

	srand(time(NULL));

	while(i < NUM_REQ){

		mes.type = TYPE_REQ;
		mes.pid = getpid();
		mes.op1 = rand()%10;
		mes.op2 = rand()%10;

		// Invio di una richiesta
		printf("[Client %d] - Invio richiesta {%d,%d} al server...\n",getpid(),mes.op1,mes.op2);
		msgsnd(id_send,&mes,sizeof(Messaggio)-sizeof(long),0);

		// In attesa della risposta
		msgrcv(id_res,&res,sizeof(Risposta)-sizeof(long),getpid(),0);
		printf("[Client %d] - Ho ricevuto il risultato %d\n",getpid(),res.risultato);

		i++;
	}


	printf("[Client %d] - Fine processo\n",getpid());

	return 0;
}
