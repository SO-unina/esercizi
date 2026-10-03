#include "common.h"

void client()
{
	int i;

	/* Messaggio del processo */
	Msg msg;
	msg.type = QUEUE_REQ;
	msg.msg = getpid();

	for(i=0; i<15; i++)
	{
		/*Invio: l'intero messaggio viene inviato, non c'e' piu' il
		  campo mtype da scorporare dalla dimensione*/
		mq_send(msgq_guest, (const char *)&msg, sizeof(Msg), 0);
		sleep(1);
	}
}
