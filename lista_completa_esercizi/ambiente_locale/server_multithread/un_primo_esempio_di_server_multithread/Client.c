#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "Header.h"

void client(int id_c, int id_s){

	int k;
	int ret;

	srand(getpid());

	msg_richiesta richiesta;
	msg_risposta risposta;

	richiesta.pid = getpid();
	richiesta.v1 = rand()%101;
	richiesta.v2 = rand()%101;

	for(k=0;k<RICHIESTE;k++){

		printf("Richiesta %d Inviata (%d, %d) [PID=%ld]\n\n", k, richiesta.v1, richiesta.v2, richiesta.pid);

		ret = msgsnd(id_c, &richiesta, sizeof(msg_richiesta)-sizeof(long), 0);

		if(ret < 0) {
			perror("Errore invio richiesta client");
			exit(1);
		}



		ret = msgrcv(id_s, &risposta, sizeof(msg_risposta)-sizeof(long), getpid(), 0);

		if(ret < 0) {
			perror("Errore ricezione risposta client");
			exit(1);
		}

		printf("Risposta %d Ricevuta (%d) [PID=%ld]\n\n", k, risposta.v3, risposta.pid);
	}

}

