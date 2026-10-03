#include "common.h"
#include <stdlib.h>
#include <stdio.h>

void server()
{
	int i;

	/* Messaggio da ricevere dal proc. client */
	Msg msg;

	/* Messaggio da inviare al proc. printer */
	Msg_buf msg_buf;

	printf("[server ] Server ready...\n");

	while(1)
	{
		for(i=0; i<BUFFER_DIM; i++)
		{
			/*Ricezione della richiesta: il buffer e' grande
			  esattamente mq_msgsize (sizeof(Msg))*/
			mq_receive(msgq_guest, (char *)&msg, sizeof(Msg), NULL);

			/*La distinzione tra richiesta normale e richiesta di
			  uscita e' realizzata dal campo "type" della struct
			  (con le code POSIX non esiste il campo mtype)*/
			if(msg.type==EXIT_REQ)
			{
				/*Richiesta di uscita*/
				msg_buf.buf[i] = -1;
				mq_send(msgq_print, (const char *)&msg_buf, sizeof(Msg_buf), 0);

				printf("[Server] Goodbye...\n");
				exit(0);
			}

			/* Accodamento della richiesta nel buffer */
			msg_buf.buf[i] = msg.msg;
		}

		/* Invio del buffer completo al proc. printer */
		mq_send(msgq_print, (const char *)&msg_buf, sizeof(Msg_buf), 0);
	}
}
