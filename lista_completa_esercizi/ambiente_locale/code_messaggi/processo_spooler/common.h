#ifndef _COMMON_
#define _COMMON_

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

/* Rinomino Tipi di Messaggi*/
#define QUEUE_REQ 1
#define PRINT_REQ 2
#define EXIT_REQ  3

/* Tipo Buffer di PID */
#define BUFFER_DIM 10
typedef pid_t Buf[BUFFER_DIM];

#define SIZEOF(x) sizeof(x)-sizeof(long)

/* Messaggio Richiesta del Client */
typedef struct
{
	long type;
	pid_t msg;
} Msg;

/* Messaggio Richiesta del Server */
typedef struct
{
	long type;
	Buf buf; 
} Msg_buf;

/* ID Strutture condivise IPC */
extern int msgq_guest;
extern int msgq_print;

/* Prototipi delle funzioni dei processi */
void printer();
void server();
void client();

#endif
