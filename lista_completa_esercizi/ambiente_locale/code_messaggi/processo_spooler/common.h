#ifndef _COMMON_
#define _COMMON_

#include <sys/types.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>

/* Nomi delle code POSIX (devono iniziare con '/'):
 * una per le richieste dei client verso il server, una per i buffer
 * inviati dal server al printer. Con le code POSIX il campo "mtype"
 * non esiste: le richieste di stampa viaggiano su una coda dedicata,
 * mentre la distinzione tra richiesta normale e richiesta di uscita
 * e' realizzata da un normale campo "tipo" nella struct. */
#define QUEUE_RICHIESTE "/spooler_richieste"
#define QUEUE_STAMPA    "/spooler_stampa"

/* Numero massimo di messaggi accodabili */
#define MAX_MESSAGGI 10

/* Tipi di messaggi sulla coda delle richieste */
#define QUEUE_REQ 1
#define EXIT_REQ  3

/* Tipo Buffer di PID */
#define BUFFER_DIM 10
typedef pid_t Buf[BUFFER_DIM];

/* Messaggio Richiesta del Client */
typedef struct
{
	int type;
	pid_t msg;
} Msg;

/* Messaggio del Server verso il Printer (solo il buffer di PID:
 * la coda dedicata rende superfluo un campo tipo) */
typedef struct
{
	Buf buf;
} Msg_buf;

/* Descrittori delle code di messaggi POSIX (ereditati dai figli
 * attraverso la fork) */
extern mqd_t msgq_guest;
extern mqd_t msgq_print;

/* Prototipi delle funzioni dei processi */
void printer();
void server();
void client();

#endif
