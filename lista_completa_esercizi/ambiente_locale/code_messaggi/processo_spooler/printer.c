#include "common.h"
#include <stdlib.h>

void printer()
{
	int counter = 1, i;
	Msg_buf msg_buf;

	printf("[printer] Printer Ready...\n");

	while(1)
	{
		/* Il buffer di ricezione deve essere grande esattamente
		   mq_msgsize (sizeof(Msg_buf)): con un buffer piu' piccolo
		   mq_receive fallisce con EMSGSIZE. La coda dedicata alle
		   stampe rende superflua la ricezione selettiva per tipo. */
		mq_receive(msgq_print, (char *)&msg_buf, sizeof(Msg_buf), NULL);

		for(i=0; i<BUFFER_DIM; i++)
		{
			if(msg_buf.buf[i]<0) //Termina la stampa se trova un pid<0
			{
				printf("[printer] Goodbye...\n");
				exit(0);
			}

			printf("{printer}\t[%u] %u\n", counter++, msg_buf.buf[i]);
		}
	}
}
