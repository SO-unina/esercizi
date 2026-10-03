#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

#define NUM_CLIENT 3
#define NUM_SERVER 1

int main(){
	int i,status;
	pid_t pid;

	/* Il programma principale crea la coda delle richieste, con
	 * attributi espliciti e rimozione preventiva dei residui.
	 * Le code di risposta, una per client, sono create dai client. */
	struct mq_attr attr = {0};
	attr.mq_maxmsg = MAX_MESSAGGI;
	attr.mq_msgsize = sizeof(Messaggio);

	mq_unlink(CODA_RICHIESTE);

	mqd_t id_send = mq_open(CODA_RICHIESTE, O_CREAT | O_WRONLY, 0664, &attr);

	if(id_send == (mqd_t)-1){
		perror("[PADRE] - Errore mq_open");
		exit(1);
	}

	for( i = 0; i < NUM_CLIENT+NUM_SERVER; i++){

		pid = fork();
		sleep(2);

		if(pid == 0){
			//srand(time(NULL));
			if(i == NUM_CLIENT+NUM_SERVER -1){
				printf("[SERVER %d] - Sono stato creato...\n",getpid());
				execl("./server","server",(char *)NULL);
				exit(0);
			}else{
				printf("Sono il client %d\n",getpid());
				execl("./client","client",(char *)NULL);
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
	chiusura.pid = getpid();
	chiusura.op1 = -1;
	chiusura.op2 = -1;

	mq_send(id_send,(const char *)&chiusura,sizeof(Messaggio),0);

	wait(&status);

	if (WIFEXITED(status)) {
    		printf("[PADRE] - Figlio terminato con stato %d\n",status);
  	}

	/* Chiusura e rimozione della coda delle richieste */
	mq_close(id_send);
	mq_unlink(CODA_RICHIESTE);

	printf("[PADRE] - Fine elaborazione...\n");

	return 0;
}
