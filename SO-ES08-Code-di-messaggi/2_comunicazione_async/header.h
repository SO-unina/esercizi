				/*-----HEADER FILE----*/
#ifndef __HEADER
#define __HEADER

#include <mqueue.h>

/* Nome della coda POSIX (visibile in /dev/mqueue) */
#define QUEUE_NAME "/so_es08_async"

/* Numero massimo di messaggi in coda */
#define MAX_MESSAGGI 10

typedef char msg[40];

/* Con le code POSIX il messaggio non richiede alcun campo "tipo":
 * e' un semplice blocco di byte definito dal programmatore. */
typedef struct {
	msg mess;
} Messaggio;

void Produttore(mqd_t queue, char *m);
void Consumatore(mqd_t queue);
void printMsgInfo(mqd_t queue);
#endif
