#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "p.h"


int main() {

	printf("Processo P1 avviato\n");


	/* Apertura per nome delle code gia' create da start:
	 * P1 invia gli operandi sulle code dedicate di P2, P3 e P4,
	 * e riceve i risultati sulla propria coda. */
	mqd_t id_op_p2 = mq_open(QUEUE_OP_P2, O_WRONLY);

	if(id_op_p2 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}

	mqd_t id_op_p3 = mq_open(QUEUE_OP_P3, O_WRONLY);

	if(id_op_p3 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}

	mqd_t id_op_p4 = mq_open(QUEUE_OP_P4, O_WRONLY);

	if(id_op_p4 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}

	mqd_t id_risultati_1 = mq_open(QUEUE_RIS_P1, O_RDONLY);

	if(id_risultati_1 == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}


	srand(time(NULL));


	int i;

	for(i=0; i<3; i++) {


		struct msg_operandi op;
		struct msg_risposta ris;

		int ret;

		int a = rand() % 10;
		int b = rand() % 10;
		int c = rand() % 10;
		int d = rand() % 10;
		int e = rand() % 10;
		int f = rand() % 10;
		int g = rand() % 10;
		int h = rand() % 10;

		int r1 = 0, r2 = 0, r3 = 0, risultato;



		printf("[P1] OPERANDI: a=%d, b=%d, c=%d, d=%d, e=%d, f=%d, g=%d, h=%d\n", a, b, c, d, e, f, g, h);


		op.operandi[0] = a;
		op.operandi[1] = b;

		printf("[P1] SEND P2\n");

		ret = mq_send(id_op_p2, (const char *)&op, sizeof(op), 0);

		if(ret<0) {
			perror("mq_send fallita");
			exit(1);
		}



		op.operandi[0] = c;
		op.operandi[1] = d;
		op.operandi[2] = e;
		op.operandi[3] = f;

		printf("[P1] SEND P3\n");

		ret = mq_send(id_op_p3, (const char *)&op, sizeof(op), 0);

		if(ret<0) {
			perror("mq_send fallita");
			exit(1);
		}



		op.operandi[0] = g;
		op.operandi[1] = h;

		printf("[P1] SEND P4\n");

		ret = mq_send(id_op_p4, (const char *)&op, sizeof(op), 0);

		if(ret<0) {
			perror("mq_send fallita");
			exit(1);
		}



		/* Ricezione dei 3 risultati. Con le code POSIX non esiste la
		 * ricezione selettiva per tipo: i risultati arrivano in un
		 * ordine qualsiasi e vengono smistati in base al campo
		 * "mittente" del messaggio. Il buffer di ricezione e' grande
		 * esattamente mq_msgsize. */
		int j;

		for(j=0; j<3; j++) {

			printf("[P1] RECEIVE\n");

			ret = mq_receive(id_risultati_1, (char *)&ris, sizeof(ris), NULL);

			if(ret<0) {
				perror("mq_receive fallita");
				exit(1);
			}

			switch(ris.mittente) {
				case P2:
					printf("[P1] RICEVUTO DA P2\n");
					r1 = ris.risposta;
					break;
				case P3:
					printf("[P1] RICEVUTO DA P3\n");
					r2 = ris.risposta;
					break;
				case P4:
					printf("[P1] RICEVUTO DA P4\n");
					r3 = ris.risposta;
					break;
				default:
					printf("[P1] MITTENTE NON RICONOSCIUTO\n");
					exit(1);
			}
		}



		printf("[P1] RISULTATI INTERMEDI: r1=%d, r2=%d, r3=%d\n", r1, r2, r3);




		risultato = r1 + r2 + r3;


		printf("[P1] RISULTATO: %d\n", risultato);

	}


	mq_close(id_op_p2);
	mq_close(id_op_p3);
	mq_close(id_op_p4);
	mq_close(id_risultati_1);

	return 0;
}
