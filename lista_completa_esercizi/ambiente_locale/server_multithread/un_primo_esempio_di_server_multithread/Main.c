#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <mqueue.h>

#include "Header.h"




int main(){

	pid_t pidc, pids;
	int i;
	int ret;

	/* Attributi espliciti della coda delle richieste */
	struct mq_attr attr = {0};
	attr.mq_maxmsg = MAX_MESSAGGI;
	attr.mq_msgsize = sizeof(msg_richiesta);

	/* Rimozione preventiva di eventuali residui di esecuzioni precedenti */
	mq_unlink(CODA_RICHIESTE);

	mqd_t id_c = mq_open(CODA_RICHIESTE, O_CREAT | O_RDWR, 0664, &attr);

	if(id_c == (mqd_t)-1) {
		perror("Errore allocazione coda");
		exit(1);
	}

	/* Il descrittore della coda viene ereditato dai figli con la fork.
	 * Le code di risposta, una per client, sono create dai client stessi. */

	for(i=0;i<CLIENT;i++){

		pidc = fork();

		if(pidc<0) {
			perror("Errore fork client");
			exit(1);
		}

		if(pidc==0){
			client(id_c);
			exit(0);
		}
	}


	pids = fork();

	if(pids<0) {
		perror("Errore fork server");
		exit(1);
	}

	if(pids==0){
		server(id_c);
		exit(0);
	}


	for(i=0;i<CLIENT;i++) {
		wait(0);
	}


	msg_richiesta uscita;
	uscita.v1 = -1;
	uscita.v2 = -1;
	uscita.pid = getpid();

	ret = mq_send(id_c, (const char *)&uscita, sizeof(msg_richiesta), 0);
	if(ret < 0) {
		perror("Errore invio messaggio di terminazione");
		exit(1);
	}

	wait(0);

	mq_close(id_c);
	mq_unlink(CODA_RICHIESTE);

	return 0;

}
