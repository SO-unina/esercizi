#include "common.h"

void printer()
{
	int counter = 1, i;
	Msg_buf msg_buf;

	printf("[printer] Printer Ready...\n");

	while(1)
	{
		msgrcv(msgq_print, &msg_buf, SIZEOF(Msg_buf), PRINT_REQ,0);

		for(i=0; i<BUFFER_DIM; i++)
		{
			if(msg_buf.buf[i]<0) //Termina la stampa se trova un pid<0
			{
				printf("[printer] Goodbye...\n");
				_exit(0);
			}

			printf("{printer}\t[%u] %u\n", counter++, msg_buf.buf[i]);
		}
	}
}
