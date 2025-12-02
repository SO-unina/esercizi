#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "p.h"


int main() {

	printf("Processo P1 avviato\n");



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

		int r1, r2, r3, risultato;



		printf("[P1] OPERANDI: a=%d, b=%d, c=%d, d=%d, e=%d, f=%d, g=%d, h=%d\n", a, b, c, d, e, f, g, h);


		op.processo = P2;
		op.operandi[0] = a;
		op.operandi[1] = b;

		printf("[P1] SEND P2\n");

		ret = msgsnd(id_operandi_1, &op, sizeof(struct msg_operandi)-sizeof(long), 0);

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}



		op.processo = P3;
		op.operandi[0] = c;
		op.operandi[1] = d;
		op.operandi[2] = e;
		op.operandi[3] = f;

		printf("[P1] SEND P3\n");

		ret = msgsnd(id_operandi_1, &op, sizeof(struct msg_operandi)-sizeof(long), 0);

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}




		op.processo = P4;
		op.operandi[0] = g;
		op.operandi[1] = h;

		printf("[P1] SEND P4\n");

		ret = msgsnd(id_operandi_1, &op, sizeof(struct msg_operandi)-sizeof(long), 0);

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}




		printf("[P1] RECEIVE P2\n");

		ret = msgrcv(id_risultati_1, &ris, sizeof(struct msg_risposta)-sizeof(long), P2, 0);

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		r1 = ris.risposta;




		printf("[P1] RECEIVE P3\n");

		ret = msgrcv(id_risultati_1, &ris, sizeof(struct msg_risposta)-sizeof(long), P3, 0);

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		r2 = ris.risposta;




		printf("[P1] RECEIVE P4\n");

		ret = msgrcv(id_risultati_1, &ris, sizeof(struct msg_risposta)-sizeof(long), P4, 0);

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		r3 = ris.risposta;





		printf("[P1] RISULTATI INTERMEDI: r1=%d, r2=%d, r3=%d\n", r1, r2, r3);




		risultato = r1 + r2 + r3;


		printf("[P1] RISULTATO: %d\n", risultato);

	}


	return 0;
}
