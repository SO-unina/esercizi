#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	printf("Processo P3 avviato\n");


	/* P3 riceve gli operandi sulla propria coda dedicata, delega i
	 * calcoli a P5 e P6 tramite le loro code, riceve i risultati
	 * parziali sulla propria coda dei risultati e invia il risultato
	 * finale sulla coda dei risultati di P1. */
	mqd_t id_op_p3 = mq_open(QUEUE_OP_P3, O_RDONLY);

	if(id_op_p3 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}

	mqd_t id_risultati_1 = mq_open(QUEUE_RIS_P1, O_WRONLY);

	if(id_risultati_1 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}

	mqd_t id_op_p5 = mq_open(QUEUE_OP_P5, O_WRONLY);

	if(id_op_p5 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}

	mqd_t id_op_p6 = mq_open(QUEUE_OP_P6, O_WRONLY);

	if(id_op_p6 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}

	mqd_t id_risultati_3 = mq_open(QUEUE_RIS_P3, O_RDONLY);

	if(id_risultati_3 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}


	int i;

	for(i=0; i<3; i++) {


		struct msg_operandi op;
		struct msg_risposta ris;

		int ret;

		int c, d, e, f, r2, r4 = 0, r5 = 0;



		printf("[P3] RECEIVE P1\n");

		ret = mq_receive(id_op_p3, (char *)&op, sizeof(op), NULL);

		if(ret<0) {
			perror("mq_receive fallita");
			exit(1);
		}

		c = op.operandi[0];
		d = op.operandi[1];
		e = op.operandi[2];
		f = op.operandi[3];

		printf("[P3] OPERANDI: c=%d, d=%d, e=%d, f=%d\n", c, d, e, f);




		printf("[P3] SEND P5\n");

		op.operandi[0] = c;
		op.operandi[1] = d;

		ret = mq_send(id_op_p5, (const char *)&op, sizeof(op), 0);

		if(ret<0) {
			perror("mq_send fallita");
			exit(1);
		}




		printf("[P3] SEND P6\n");

		op.operandi[0] = e;
		op.operandi[1] = f;

		ret = mq_send(id_op_p6, (const char *)&op, sizeof(op), 0);

		if(ret<0) {
			perror("mq_send fallita");
			exit(1);
		}




		/* Ricezione dei 2 risultati parziali, in ordine qualsiasi,
		 * smistati in base al campo "mittente" (con le code POSIX non
		 * esiste la ricezione selettiva per tipo). */
		int j;

		for(j=0; j<2; j++) {

			printf("[P3] RECEIVE\n");

			ret = mq_receive(id_risultati_3, (char *)&ris, sizeof(ris), NULL);

			if(ret<0) {
				perror("mq_receive fallita");
				exit(1);
			}

			switch(ris.mittente) {
				case P5:
					printf("[P3] RICEVUTO DA P5\n");
					r4 = ris.risposta;
					break;
				case P6:
					printf("[P3] RICEVUTO DA P6\n");
					r5 = ris.risposta;
					break;
				default:
					printf("[P3] MITTENTE NON RICONOSCIUTO\n");
					exit(1);
			}
		}




		printf("[P3] RISULTATI INTERMEDI: r4=%d, r5=%d\n", r4, r5);





		r2 = r4 * r5;

		printf("[P3] RISULTATO: %d\n", r2);






		ris.mittente = P3;
		ris.risposta = r2;

		printf("[P3] SEND P1\n");

		ret = mq_send(id_risultati_1, (const char *)&ris, sizeof(ris), 0);

		if(ret<0) {
			perror("mq_send fallita");
			exit(1);
		}


	}


	mq_close(id_op_p3);
	mq_close(id_risultati_1);
	mq_close(id_op_p5);
	mq_close(id_op_p6);
	mq_close(id_risultati_3);

	return 0;
}
