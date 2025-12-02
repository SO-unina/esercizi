#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	printf("Processo P2 avviato\n");



	key_t queue_operandi_1  = ftok(FTOK_PATH_Q_OP_1, FTOK_CHAR_Q_OP_1);
	key_t queue_risultati_1 = ftok(FTOK_PATH_Q_RIS_1, FTOK_CHAR_Q_RIS_1);



	int id_operandi_1 = msgget(queue_operandi_1, 0644);

	if(id_operandi_1 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



	int id_risultati_1 = msgget(queue_risultati_1, 0644);

	if(id_risultati_1 < 0) {
		perror("Msgget fallita");
		exit(1);
	}




	int i;

	for(i=0; i<3; i++) {


		struct msg_operandi op;
		struct msg_risposta ris;

		int ret;

		int a, b, r1;



		printf("[P2] RECEIVE P1\n");

		ret = msgrcv(id_operandi_1, &op, sizeof(struct msg_operandi)-sizeof(long), P2, 0);

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		a = op.operandi[0];
		b = op.operandi[1];

		printf("[P2] OPERANDI: a=%d, b=%d\n", a, b);




		r1 = a * b;

		printf("[P2] RISULTATO: %d\n", r1);




		ris.processo = P2;
		ris.risposta = r1;

		printf("[P2] SEND P1\n");

		ret = msgsnd(id_risultati_1, &ris, sizeof(struct msg_risposta)-sizeof(long), 0);

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}


	}


	return 0;
}
