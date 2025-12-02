#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "header.h"


int main(int argc, char *argv[]) {

	pid_t pid;
	int i;
	
	// Controllo gli argomenti passati
	if(argc<2){
		printf("Errore: Fornire come parametri di ingresso 2 caratteri separati da spazio\n");
        printf("Esempio: %s a b\n", argv[0]);
		_exit(1);
	}

	char firstChar = *argv[1];
	char secondChar = *argv[2];

	printf("I caratteri inseriti sono: %c %c\n", firstChar, secondChar);

	// Creo le chiavi per le due code di messaggi
	key_t queue_sender  = ftok(FTOK_PATH, firstChar);
	key_t queue_receiver = ftok(FTOK_PATH, secondChar);

	// Creo le due code	
	int id_queue_sender = msgget(queue_sender, IPC_CREAT|0644);

	if(id_queue_sender < 0) {
		perror("Msgget fallita");
		exit(1);
	}

        int id_queue_receiver = msgget(queue_receiver, IPC_CREAT|0644);

	if(id_queue_receiver < 0) {
		perror("Msgget fallita");
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

	// Rimuovo le code
	msgctl(id_queue_sender, IPC_RMID, 0);
	msgctl(id_queue_receiver, IPC_RMID, 0);

}
