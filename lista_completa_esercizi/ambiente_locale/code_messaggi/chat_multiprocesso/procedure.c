#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

void Sender(int id_queue_receiver, int id_queue_sender){
	
	struct mesg mess; // struct del messaggio
	int ret;

	while(1){

		// Prelevo il messaggio da inviare
		printf("Inserisci il messaggio da inviare [max 20 caratteri]\n");				
		scanf("%s",mess.message);

		// Setto il tipo
		mess.msgType=TYPE;

		// Verifico se il messaggio è exit
		if(strcmp(mess.message,"exit")==0){

			// Essendo exit, lo invio sulla seconda coda e termino
			ret = msgsnd(id_queue_receiver, &mess, sizeof(struct mesg)-sizeof(long), 0);
			if(ret<0) {
				//perror("Msgsnd fallita");
				_exit(1);
			}
            
			printf("[SENDER] inviato: %s\n", mess.message);
			
            _exit(1);

		}else{

			// Non essendo exit, lo invio sulla prima coda
			ret = msgsnd(id_queue_sender, &mess, sizeof(struct mesg)-sizeof(long), 0);
			
            if(ret<0) {
				//perror("Msgsnd fallita");
				_exit(1);
			}
			
            printf("[SENDER] inviato: %s\n",mess.message);
		}
	}

}

void Receiver(int id_queue_receiver){

	struct mesg mess; // struct del messaggio
	int ret;

	while(1){

		// Mi metto in attesa di un messaggio sulla seconda coda
		ret = msgrcv(id_queue_receiver, &mess, sizeof(struct mesg)-sizeof(long), TYPE, 0);
        
		if(ret<0) {
			//perror("Msgrcv fallita");
			_exit(1);
		}

		printf("[RECEIVER] ricevuto: %s\n",mess.message);

		// Se il messaggio ricevuto è exit termino
		if(strcmp(mess.message,"exit")==0)
			_exit(1);

	}

}
