#ifndef __HEADER__
#define __HEADER__

#include <mqueue.h>

/* Prefisso per i nomi delle code di messaggi POSIX.
 * Il nome completo viene costruito concatenando il carattere passato
 * sulla linea di comando (es. carattere 'a' -> coda "/chat_a").
 * I nomi delle code POSIX devono iniziare con '/'. */
#define QUEUE_PREFIX "/chat_"

/* Dimensione massima del nome di una coda */
#define QUEUE_NAME_SIZE 32

/* Numero massimo di messaggi accodabili */
#define MAX_MESSAGGI 10

/* Struct relativa ai messaggi.
 * Nota: con le code POSIX non esiste il campo "mtype" (long) delle
 * code System V: il messaggio contiene solo i dati applicativi. */
struct mesg {
	char message[20];	// messaggio effettivo
};

void Sender(mqd_t, mqd_t);
void Receiver(mqd_t);

#endif // __HEADER__
