#include "common.h"

void client()
{
	int i;

	/* Messaggio del processo */
	Msg msg = {
		.type = QUEUE_REQ,
		.msg  = getpid()
	};

	for(i=0; i<15; i++)
	{
		/*Invio*/
		msgsnd(msgq_guest, &msg, SIZEOF(Msg),0);
		sleep(1);
	}
}
