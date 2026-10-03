#include "common.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define NR_CLIENT  5

/* Descrittori - Code di Messaggi POSIX */
mqd_t msgq_guest;
mqd_t msgq_print;

int main()
{
	int i;

	/* Attributi espliciti delle due code: capacita' e dimensione del
	   singolo messaggio (diversa per le due code) */
	struct mq_attr attr_guest;
	attr_guest.mq_flags = 0;
	attr_guest.mq_maxmsg = MAX_MESSAGGI;
	attr_guest.mq_msgsize = sizeof(Msg);
	attr_guest.mq_curmsgs = 0;

	struct mq_attr attr_print;
	attr_print.mq_flags = 0;
	attr_print.mq_maxmsg = MAX_MESSAGGI;
	attr_print.mq_msgsize = sizeof(Msg_buf);
	attr_print.mq_curmsgs = 0;

	/* Creazione code di messaggi POSIX, con mq_unlink() preventivo
	   per rimuovere eventuali residui di esecuzioni precedenti */
	mq_unlink(QUEUE_RICHIESTE);
	mq_unlink(QUEUE_STAMPA);

	msgq_guest = mq_open(QUEUE_RICHIESTE, O_CREAT | O_RDWR, 0664, &attr_guest);

	if(msgq_guest == (mqd_t)-1)
	{
		perror("Errore mq_open() coda richieste");
		exit(1);
	}

	msgq_print = mq_open(QUEUE_STAMPA, O_CREAT | O_RDWR, 0664, &attr_print);

	if(msgq_print == (mqd_t)-1)
	{
		perror("Errore mq_open() coda stampa");
		exit(1);
	}


	/*Creazione processo 'printer'*/
	if(!fork())
	{
		printer();
		exit(0);
	}

	/*Creazione processo 'server'*/
	if(!fork())
	{
		server();
		exit(0);
	}

	/*Creazione processi 'client'*/
	for(i=0; i<NR_CLIENT; i++)
		if(!fork())
		{
			client();
			exit(0);
		}

	/*Attesa terminazione dei client*/
	for(i=0; i<NR_CLIENT; i++)
		wait(0);

	/*Invio messaggio di terminazione del server*/
	Msg quit_server = { .type = EXIT_REQ, .msg = 0 };
	mq_send(msgq_guest, (const char *)&quit_server, sizeof(Msg), 0);

	/*Attesa terminazione processi 'server' e 'printer'*/
	for(i=0; i<2; i++)
		wait(0);

	/* Rimozione code e uscita: mq_close() chiude il descrittore,
	   mq_unlink() rimuove il nome. Il nome sparisce subito, la coda
	   viene distrutta quando tutti i processi che la avevano aperta
	   la chiudono. */
	mq_close(msgq_guest);
	mq_close(msgq_print);

	mq_unlink(QUEUE_RICHIESTE);
	mq_unlink(QUEUE_STAMPA);

	return 0;
}
