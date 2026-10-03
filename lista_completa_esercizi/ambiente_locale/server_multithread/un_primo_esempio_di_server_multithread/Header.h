#ifndef HEADER_H
#define HEADER_H

#include <sys/types.h>
#include <mqueue.h>

#define RICHIESTE 5
#define CLIENT 3

/* Nome della coda POSIX delle richieste, condivisa tra tutti i processi.
 * Sostituisce l'identificatore numerico delle code System V. */
#define CODA_RICHIESTE "/primo_server_mt_richieste"

/* Ogni client crea una propria coda di risposta, il cui nome contiene
 * il PID. Sostituisce la ricezione selettiva con mtype=PID: il server
 * apre la coda del client destinatario e vi invia la risposta. */
#define CODA_RISPOSTA_FMT "/primo_server_mt_risp_%d"
#define NOME_CODA_MAX 64

#define MAX_MESSAGGI 10

/* Con le code POSIX non esiste il campo "mtype": il PID del client
 * e' un normale campo del messaggio. */
typedef struct{
	pid_t pid;
	int v1;
	int v2;
} msg_richiesta;

typedef struct{
	pid_t pid;
	int v3;
} msg_risposta;

void client(mqd_t coda_richieste);
void server(mqd_t coda_richieste);
void* Prodotto(void*);

#endif
