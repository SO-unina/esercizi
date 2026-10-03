#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	printf("Processo P5 avviato\n");


	/* P5 riceve gli operandi sulla propria coda dedicata e invia il
	 * risultato sulla coda dei risultati di P3. */
	mqd_t id_op_p5 = mq_open(QUEUE_OP_P5, O_RDONLY);

	if(id_op_p5 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}

	mqd_t id_risultati_3 = mq_open(QUEUE_RIS_P3, O_WRONLY);

	if(id_risultati_3 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}


	int i;

	for(i=0; i<3; i++) {


		struct msg_operandi op;
		struct msg_risposta ris;

		int ret;

		int c, d, r4;



		printf("[P5] RECEIVE P3\n");

		ret = mq_receive(id_op_p5, (char *)&op, sizeof(op), NULL);

		if(ret<0) {
			perror("mq_receive fallita");
			exit(1);
		}

		c = op.operandi[0];
		d = op.operandi[1];

		printf("[P5] OPERANDI: c=%d, d=%d\n", c, d);




		r4 = c + d;

		printf("[P5] RISULTATO: %d\n", r4);




		ris.mittente = P5;
		ris.risposta = r4;

		printf("[P5] SEND P3\n");

		ret = mq_send(id_risultati_3, (const char *)&ris, sizeof(ris), 0);

		if(ret<0) {
			perror("mq_send fallita");
			exit(1);
		}


	}


	mq_close(id_op_p5);
	mq_close(id_risultati_3);

	return 0;
}
