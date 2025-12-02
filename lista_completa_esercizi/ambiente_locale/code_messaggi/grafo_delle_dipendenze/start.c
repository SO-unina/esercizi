#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	pid_t pid;
	int i;



        key_t queue_operandi_1  = ftok(FTOK_PATH_Q_OP_1, FTOK_CHAR_Q_OP_1);
	key_t queue_risultati_1 = ftok(FTOK_PATH_Q_RIS_1, FTOK_CHAR_Q_RIS_1);
	key_t queue_operandi_2  = ftok(FTOK_PATH_Q_OP_2, FTOK_CHAR_Q_OP_2);
	key_t queue_risultati_2 = ftok(FTOK_PATH_Q_RIS_2, FTOK_CHAR_Q_RIS_2);



        int id_operandi_1 = msgget(queue_operandi_1, IPC_CREAT|0644);

	if(id_operandi_1 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



        int id_risultati_1 = msgget(queue_risultati_1, IPC_CREAT|0644);

	if(id_risultati_1 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



        int id_operandi_2 = msgget(queue_operandi_2, IPC_CREAT|0644);

	if(id_operandi_2 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



	int id_risultati_2 = msgget(queue_risultati_2, IPC_CREAT|0644);

	if(id_risultati_2 < 0) {
		perror("Msgget fallita");
		exit(1);
	}




	pid = fork();

	if(pid==0) {

		execl("./p1", "p1", NULL);

		perror("Exec fallita");
		exit(1);

	} else if(pid<0) {

		perror("Fork fallita");
		exit(1);
	}



	pid = fork();

	if(pid==0) {

		execl("./p2", "p2", NULL);

		perror("Exec fallita");
		exit(1);

	} else if(pid<0) {

		perror("Fork fallita");
		exit(1);
	}



	pid = fork();

	if(pid==0) {

		execl("./p3", "p3", NULL);

		perror("Exec fallita");
		exit(1);

	} else if(pid<0) {

		perror("Fork fallita");
		exit(1);
	}



	pid = fork();

	if(pid==0) {

		execl("./p4", "p4", NULL);

		perror("Exec fallita");
		exit(1);

	} else if(pid<0) {

		perror("Fork fallita");
		exit(1);
	}



	pid = fork();

	if(pid==0) {

		execl("./p5", "p5", NULL);

		perror("Exec fallita");
		exit(1);

	} else if(pid<0) {

		perror("Fork fallita");
		exit(1);
	}



	pid = fork();

	if(pid==0) {

		execl("./p6", "p6", NULL);

		perror("Exec fallita");
		exit(1);

	} else if(pid<0) {

		perror("Fork fallita");
		exit(1);
	}



	for(i=0; i<6; i++) {
		wait(NULL);
	}



        msgctl(id_operandi_1, IPC_RMID, 0);
	msgctl(id_risultati_1, IPC_RMID, 0);
	msgctl(id_operandi_2, IPC_RMID, 0);
	msgctl(id_risultati_2, IPC_RMID, 0);


	return 0;
}

