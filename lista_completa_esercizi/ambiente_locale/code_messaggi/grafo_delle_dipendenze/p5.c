#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	printf("Processo P5 avviato\n");



	key_t queue_operandi_2  = ftok(FTOK_PATH_Q_OP_2, FTOK_CHAR_Q_OP_2);
	key_t queue_risultati_2 = ftok(FTOK_PATH_Q_RIS_2, FTOK_CHAR_Q_RIS_2);



	int id_operandi_2 = msgget(queue_operandi_2, 0644);

	if(id_operandi_2 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



	int id_risultati_2 = msgget(queue_risultati_2, 0644);

	if(id_risultati_2 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



	int i;

	for(i=0; i<3; i++) {


		struct msg_operandi op;
		struct msg_risposta ris;

		int ret;

		int c, d, r4;



		printf("[P5] RECEIVE P3\n");

		ret = msgrcv(id_operandi_2, &op, sizeof(struct msg_operandi)-sizeof(long), P5, 0);

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		c = op.operandi[0];
		d = op.operandi[1];

		printf("[P5] OPERANDI: c=%d, d=%d\n", c, d);




		r4 = c + d;

		printf("[P5] RISULTATO: %d\n", r4);




		ris.processo = P5;
		ris.risposta = r4;

		printf("[P5] SEND P3\n");

		ret = msgsnd(id_risultati_2, &ris, sizeof(struct msg_risposta)-sizeof(long), 0);

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}


	}


	return 0;
}
