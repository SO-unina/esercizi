#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

#define NUM_CLIENT 3
#define NUM_SERVER 1

int main(){
	int i,status;
	int id_send,id_res;
	pid_t pid;

	key_t key_send,key_res;
	key_send = ftok(FTOK_QUEUE_CMD_PATH,FTOK_QUEUE_CMD_TOK);
	key_res = ftok(FTOK_QUEUE_RES_PATH,FTOK_QUEUE_RES_TOK);

	id_send = msgget(key_send,IPC_CREAT | 0664);
	id_res = msgget(key_res,IPC_CREAT | 0664);
	
	for( i = 0; i < NUM_CLIENT+NUM_SERVER; i++){

		pid = fork();
		sleep(2);

		if(pid == 0){
			//srand(time(NULL));
			if(i == NUM_CLIENT+NUM_SERVER -1){
				printf("[SERVER %d] - Sono stato creato...\n",getpid());
				execve("./server",NULL,NULL);
				exit(0);
			}else{
				printf("Sono il client %d\n",getpid());
				execve("./client",NULL,NULL);
				exit(0);
			}
		}
	}

	for(i = 0; i < NUM_CLIENT;i++){

		wait(&status);

		if (WIFEXITED(status)) {
    			printf("[PADRE] - Figlio terminato con stato %d\n",status);
  		}
	}

	printf("[PADRE] - Ho terminato di aspettare i figli client...\n");
	sleep(3);

	printf("[PADRE] - Mando un messaggio di chiusura al server...\n");

	Messaggio chiusura;
	chiusura.type = TYPE_REQ;
	chiusura.pid = getpid();
	chiusura.op1 = -1;
	chiusura.op2 = -1;

	msgsnd(id_send,&chiusura,sizeof(Messaggio)-sizeof(long),0);

	wait(&status);	

	if (WIFEXITED(status)) {
    		printf("[PADRE] - Figlio terminato con stato %d\n",status);
  	}

	printf("[PADRE] - Fine elaborazione...\n");

	return 0;	
}
