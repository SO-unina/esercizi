#ifndef __HEADER
#define __HEADER

//Funzioni che ci servono
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#define FTOK_QUEUE_CMD_PATH "."
#define FTOK_QUEUE_CMD_TOK 'A'
#define FTOK_QUEUE_RES_PATH "."
#define FTOK_QUEUE_RES_TOK 'B'
#define MAX 2
#define TYPE_REQ 1

typedef struct{
	long type;
	pid_t pid;
	int op1;
	int op2;
}Messaggio;

typedef struct{
	long type;
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
