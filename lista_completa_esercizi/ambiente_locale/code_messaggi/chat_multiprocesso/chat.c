#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

#include "header.h"


int main(int argc, char *argv[]) {

	pid_t pid;
	int i;

	// Controllo gli argomenti passati
	if(argc < 3){
		printf("Errore: Fornire come parametri di ingresso 2 caratteri separati da spazio\n");
		printf("Esempio: %s a b\n", argv[0]);
		exit(1);
	}

	char firstChar = *argv[1];
	char secondChar = *argv[2];

	printf("I caratteri inseriti sono: %c %c\n", firstChar, secondChar);

	/* Costruisco i nomi delle due code POSIX a partire dai caratteri:
	 * due utenti che usano gli stessi caratteri (scambiati) condividono
	 * le stesse code. Non servono piu' le chiavi System V. */
	char name_sender[QUEUE_NAME_SIZE];
	char name_receiver[QUEUE_NAME_SIZE];

	sprintf(name_sender, QUEUE_PREFIX "%c", firstChar);
	sprintf(name_receiver, QUEUE_PREFIX "%c", secondChar);

	/* Attributi espliciti delle code: capacita' e dimensione del
	 * singolo messaggio. */
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MAX_MESSAGGI;
	attr.mq_msgsize = sizeof(struct mesg);
	attr.mq_curmsgs = 0;

	/* Creo (o apro, se il pari l'ha gia' creata) le due code.
	 * Nota: qui NON si fa mq_unlink() preventivo, perche' la coda
	 * potrebbe essere stata gia' creata dall'altro utente della chat
	 * e va condivisa, non distrutta. */
	mqd_t id_queue_sender = mq_open(name_sender, O_CREAT | O_RDWR, 0644, &attr);

	if(id_queue_sender == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}

	mqd_t id_queue_receiver = mq_open(name_receiver, O_CREAT | O_RDWR, 0644, &attr);

	if(id_queue_receiver == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}


	// Genero i due figli, mittente e destinatario
	for(i=0; i<2; i++){

		pid = fork();

		if(pid==0){	// Processo figlio

			if(i==0)	// Sender

				Sender(id_queue_receiver, id_queue_sender);

			else	// Receiver

				Receiver(id_queue_receiver);

		}else
			if (pid <0){
				perror("Fork fallita");
				exit(1);
			}

	}

	// Attendo che i figli, mittente e destinatario, siano terminati
	for(i=0; i<2; i++) {
		wait(NULL);
	}

	/* Rimuovo le code: mq_close() chiude il descrittore, mq_unlink()
	 * rimuove il nome dal sistema. Il nome sparisce subito, la coda
	 * viene effettivamente distrutta quando tutti i processi che la
	 * usano l'hanno chiusa. Se l'altro utente ha gia' fatto
	 * mq_unlink(), la chiamata fallisce con ENOENT: non e' un errore. */
	mq_close(id_queue_sender);
	mq_close(id_queue_receiver);

	mq_unlink(name_sender);
	mq_unlink(name_receiver);

	return 0;
}
