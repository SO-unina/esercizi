#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	pid_t pid;
	int i;

	/* Attributi delle code degli operandi e dei risultati */
	struct mq_attr attr_op = {0};
	attr_op.mq_maxmsg = MAX_MESSAGGI;
	attr_op.mq_msgsize = sizeof(struct msg_operandi);

	struct mq_attr attr_ris = {0};
	attr_ris.mq_maxmsg = MAX_MESSAGGI;
	attr_ris.mq_msgsize = sizeof(struct msg_risposta);

	const char *code_op[] = { QUEUE_OP_P2, QUEUE_OP_P3, QUEUE_OP_P4,
	                          QUEUE_OP_P5, QUEUE_OP_P6 };
	const char *code_ris[] = { QUEUE_RIS_P1, QUEUE_RIS_P3 };

	/* Creazione delle code. mq_unlink() preventivo per rimuovere
	 * eventuali residui di esecuzioni precedenti. I processi P1..P6,
	 * essendo lanciati con exec(), riapriranno le code per nome. */
	for(i=0; i<5; i++) {

		mq_unlink(code_op[i]);

		mqd_t q = mq_open(code_op[i], O_CREAT | O_RDWR, 0644, &attr_op);

		if(q == (mqd_t)-1) {
			perror("mq_open fallita");
			exit(1);
		}

		mq_close(q);
	}

	for(i=0; i<2; i++) {

		mq_unlink(code_ris[i]);

		mqd_t q = mq_open(code_ris[i], O_CREAT | O_RDWR, 0644, &attr_ris);

		if(q == (mqd_t)-1) {
			perror("mq_open fallita");
			exit(1);
		}

		mq_close(q);
	}


	const char *programmi[] = { "./p1", "./p2", "./p3", "./p4", "./p5", "./p6" };
	const char *nomi[]      = { "p1", "p2", "p3", "p4", "p5", "p6" };

	for(i=0; i<6; i++) {

		pid = fork();

		if(pid==0) {

			execl(programmi[i], nomi[i], NULL);

			perror("Exec fallita");
			exit(1);

		} else if(pid<0) {

			perror("Fork fallita");
			exit(1);
		}
	}


	for(i=0; i<6; i++) {
		wait(NULL);
	}


	/* Rimozione delle code: mq_unlink() rimuove il nome, che sparisce
	 * subito; la coda viene distrutta quando tutti i processi che la
	 * avevano aperta la chiudono (qui sono gia' tutti terminati). */
	for(i=0; i<5; i++)
		mq_unlink(code_op[i]);

	for(i=0; i<2; i++)
		mq_unlink(code_ris[i]);


	return 0;
}
