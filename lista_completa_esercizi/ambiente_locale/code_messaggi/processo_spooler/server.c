#include "common.h"
#include <stdio.h>

void server()
{
	int i;
	Buf Buf;
	
	/* Messaggio da ricevere dal proc. client */
	Msg msg;

	/* Messaggio da inviare al proc. printer */
	Msg_buf msg_buf = {
		.type= PRINT_REQ
	};

	printf("[server ] Server ready...\n");
	while(1)
	{
		for(i=0; i<BUFFER_DIM; i++)
		{
			/*Ricezione della richiesta*/
			msgrcv(msgq_guest, &msg, SIZEOF(Msg), 0, 0);
			if(msg.type==EXIT_REQ)
			{
				/*Richiesta di uscita*/
				msg_buf.buf[i] = -1;
				msgsnd(msgq_print, &msg_buf, SIZEOF(Msg_buf), 0);

				printf("[Server] Goodbye...\n");
				_exit(0);
			}

			/* Accodamento della richiesta nel buffer */
			msg_buf.buf[i] = msg.msg;
		}
		
	/* Invio del buffer completo al proc. printer */
		msgsnd(msgq_print, &msg_buf, SIZEOF(Msg_buf), 0);
	}
}
