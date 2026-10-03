#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	printf("Processo P2 avviato\n");


	/* P2 riceve gli operandi sulla propria coda dedicata e invia il
	 * risultato sulla coda dei risultati di P1. */
	mqd_t id_op_p2 = mq_open(QUEUE_OP_P2, O_RDONLY);

	if(id_op_p2 == (mqd_t)-1) {
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

		int a, b, r1;



		printf("[P2] RECEIVE P1\n");

		ret = mq_receive(id_op_p2, (char *)&op, sizeof(op), NULL);

		if(ret<0) {
			perror("mq_receive fallita");
			exit(1);
		}

		a = op.operandi[0];
		b = op.operandi[1];

		printf("[P2] OPERANDI: a=%d, b=%d\n", a, b);




		r1 = a * b;

		printf("[P2] RISULTATO: %d\n", r1);




		ris.mittente = P2;
		ris.risposta = r1;

		printf("[P2] SEND P1\n");

		ret = mq_send(id_risultati_1, (const char *)&ris, sizeof(ris), 0);

		if(ret<0) {
			perror("mq_send fallita");
			exit(1);
		}


	}


	mq_close(id_op_p2);
	mq_close(id_risultati_1);

	return 0;
}
