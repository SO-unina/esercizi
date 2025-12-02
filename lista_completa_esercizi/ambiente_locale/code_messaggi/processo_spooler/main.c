#include "common.h"
#include <sys/wait.h>

#define NR_CLIENT  5

/* ID - Code di Messaggi */
int msgq_guest;
int msgq_print;

int main(int argc,char*argv[])
{
	int i;
	/*Richiesta code di messaggi IPC*/
	msgq_guest = msgget(IPC_PRIVATE, IPC_CREAT|0664);
	msgq_print = msgget(IPC_PRIVATE, IPC_CREAT|0664); 

	/*Creazione processo 'printer'*/
	if(!fork())
	{
		printer();
		_exit(0);
	}

	/*Creazione processo 'server'*/
	if(!fork())
	{
		server();
		_exit(0);
	}

	/*Creazione processi 'client'*/
	for(i=0; i<NR_CLIENT; i++)
		if(!fork())
		{
			client();
			_exit(0);
		}

	/*Attesa terminazione dei client*/
	for(i=0; i<NR_CLIENT; i++)
		wait(0);

	/*Invio messaggio di terminazione del server*/
	Msg     quit_server  = { .type = EXIT_REQ};
	msgsnd(msgq_guest, &quit_server, SIZEOF(Msg),0);

	/*Attesa terminazione processi 'server' e 'printer'*/
	for(i=0; i<2; i++)
		wait(0);

	/*Rimozione strutture IPC e uscita*/
	msgctl(msgq_guest, 0, IPC_RMID);
	msgctl(msgq_print, 0, IPC_RMID);

	return 0;
}
