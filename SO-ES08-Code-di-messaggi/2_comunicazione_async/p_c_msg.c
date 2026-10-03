/*********PRODUTTORE-CONSUMATORE MEDIANTE SCAMBIO DI MESSAGGI******/
/*Il programma gestisce la comunicazione tra due processi, modello asincrono
  viene inviato un burst di messaggi e stampato lo stato della coda
  Header file:header.h
  Programma chiamante:p_c_msg.c
  Modulo delle procedure:procedure.c
  Direttive per la compilazione dei moduli:Makefile
*/

#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include "header.h"


int main() {

	int k, status;
	pid_t pid;
	int i;
	char m[30];

	// creazione della coda di comunicazione: gli attributi
	// (n. massimo di messaggi e dimensione del messaggio)
	// vengono fissati alla creazione
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MAX_MESSAGGI;
	attr.mq_msgsize = sizeof(Messaggio);
	attr.mq_curmsgs = 0;

	// rimuove un'eventuale coda rimasta da esecuzioni precedenti
	mq_unlink(QUEUE_NAME);

	mqd_t queue = mq_open(QUEUE_NAME, O_RDWR | O_CREAT | O_EXCL, 0664, &attr);
	if (queue == (mqd_t)-1) {
		perror("errore mq_open");
		exit(1);
	}

	// generazione produttore e consumatore
	pid = fork();
	if (pid == 0) {			//processo figlio (produttore)
		printf("sono il produttore. Il mio pid %d \n", getpid());
		for (i = 0; i < 10; i++) {
			sprintf(m, "stringa %d", i);
			usleep(100);
			Produttore(queue, m);
		}
		exit(0);
	} else {
		pid = fork();		//genera il secondo figlio (consumatore)
		if (pid == 0) {
			printf("sono il figlio consumatore. Il mio pid %d \n", getpid());
			sleep(1);
			for (i = 0; i < 10; i++) {
				Consumatore(queue);
			}
			exit(0);
		}
	}

	// attesa di terminazione
	for (k = 0; k < 2; k++) {
		pid = wait(&status);
		if (pid == -1)
			perror("errore");
		else
			printf("Figlio n.ro %d e\' morto con status= %d\n", pid, status >> 8);
	}

	// chiusura del descrittore e rimozione della coda
	mq_close(queue);
	mq_unlink(QUEUE_NAME);
	return 0;
}
