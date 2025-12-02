#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	printf("Processo P6 avviato\n");



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

		int e, f, r5;



		printf("[P6] RECEIVE P3\n");

		ret = msgrcv(id_operandi_2, &op, sizeof(struct msg_operandi)-sizeof(long), P6, 0);

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		e = op.operandi[0];
		f = op.operandi[1];

		printf("[P6] OPERANDI: e=%d, f=%d\n", e, f);




		r5 = e + f;

		printf("[P6] RISULTATO: %d\n", r5);




		ris.processo = P6;
		ris.risposta = r5;

		printf("[P6] SEND P3\n");

		ret = msgsnd(id_risultati_2, &ris, sizeof(struct msg_risposta)-sizeof(long), 0);

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}


	}


	return 0;
}
