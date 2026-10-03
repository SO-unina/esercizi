#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>

#include "header.h"


int main(int argc, char **argv) {

	pid_t pid;
	int i;

	/* Crea (o riapre) la coda condivisa PRIMA di lanciare i figli,
	 * cosi' che esista di sicuro quando i figli la aprono. */
	mqd_t id_queue = apri_coda();


	pid = fork();

	if (pid == 0) {

		execl("./p1", "./p1", (char *)0);

		perror("Exec fallita");
		exit(1);

	} else if (pid < 0) {

		perror("Fork fallita");
		exit(1);
	}



	pid = fork();

	if (pid == 0) {

		execl("./p2", "./p2", (char *)0);

		perror("Exec fallita");
		exit(1);

	} else if (pid < 0) {

		perror("Fork fallita");
		exit(1);
	}



	pid = fork();

	if (pid == 0) {

		execl("./p3", "./p3", (char *)0);

		perror("Exec fallita");
		exit(1);

	} else if (pid < 0) {

		perror("Fork fallita");
		exit(1);
	}



	for (i = 0; i < 3; i++) {
		wait(NULL);
	}


	/* Rimozione della coda: il nome sparisce subito, l'oggetto
	 * quando tutti i processi lo hanno chiuso. */
	mq_close(id_queue);
	mq_unlink(QUEUE_NAME);
	return 0;
}
