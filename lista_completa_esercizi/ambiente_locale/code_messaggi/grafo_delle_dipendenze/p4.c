#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	printf("Processo P4 avviato\n");


	/* P4 riceve gli operandi sulla propria coda dedicata e invia il
	 * risultato sulla coda dei risultati di P1. */
	mqd_t id_op_p4 = mq_open(QUEUE_OP_P4, O_RDONLY);

	if(id_op_p4 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}

	mqd_t id_risultati_1 = mq_open(QUEUE_RIS_P1, O_WRONLY);

	if(id_risultati_1 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}


	int i;

	for(i=0; i<3; i++) {


		struct msg_operandi op;
		struct msg_risposta ris;

		int ret;

		int g, h, r3;



		printf("[P4] RECEIVE P1\n");

		ret = mq_receive(id_op_p4, (char *)&op, sizeof(op), NULL);

		if(ret<0) {
			perror("mq_receive fallita");
			exit(1);
		}

		g = op.operandi[0];
		h = op.operandi[1];

		printf("[P4] OPERANDI: g=%d, h=%d\n", g, h);




		r3 = g * h;

		printf("[P4] RISULTATO: %d\n", r3);




		ris.mittente = P4;
		ris.risposta = r3;

		printf("[P4] SEND P1\n");

		ret = mq_send(id_risultati_1, (const char *)&ris, sizeof(ris), 0);

		if(ret<0) {
			perror("mq_send fallita");
			exit(1);
		}


	}


	mq_close(id_op_p4);
	mq_close(id_risultati_1);

	return 0;
}
