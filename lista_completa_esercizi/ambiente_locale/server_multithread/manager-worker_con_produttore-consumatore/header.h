#ifndef __HEADER
#define __HEADER

//Funzioni che ci servono
#include <sys/types.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>

/* Nome della coda POSIX delle richieste: sostituisce la coppia
 * (percorso, chiave) delle code System V. */
#define CODA_RICHIESTE "/mw_prodcons_richieste"

/* Ogni client crea una propria coda di risposta, il cui nome contiene
 * il PID: sostituisce la ricezione selettiva con mtype=PID. */
#define CODA_RISPOSTA_FMT "/mw_prodcons_risp_%d"
#define NOME_CODA_MAX 64

#define MAX_MESSAGGI 10
#define MAX 2

/* Con le code POSIX non esiste il campo "mtype": il PID del client
 * e' un normale campo del messaggio. */
typedef struct{
	pid_t pid;
	int op1;
	int op2;
}Messaggio;

typedef struct{
	int risultato;
}Risposta;

typedef struct{
	pthread_cond_t ok_cons;
	pthread_cond_t ok_prod;
	pthread_mutex_t mutex;
	Messaggio elems[MAX];
	int count;
	int testa;
	int coda;
}Buffer;

#endif
