				/*-----HEADER FILE----*/
#ifndef __HEADER
#define __HEADER

#include <mqueue.h>

/* Nomi delle tre code POSIX: una per i dati e due di servizio
 * per realizzare il rendezvous (con POSIX non serve il campo
 * "tipo": ogni classe di messaggi ha la propria coda). */
#define QUEUE_DATI "/so_es08_sync_dati"
#define QUEUE_RTS  "/so_es08_sync_rts"   /* request-to-send */
#define QUEUE_OTS  "/so_es08_sync_ots"   /* ok-to-send */

#define MAX_MESSAGGI 10

typedef char msg[40];

typedef struct {
	msg mess;
} Messaggio;

void ReceiveBloc(Messaggio *, mqd_t);
void SendSincr(Messaggio *, mqd_t);
void initServiceQueues(void);
void removeServiceQueues(void);

void Produttore(mqd_t queue, char *);
void Consumatore(mqd_t queue);

#endif
