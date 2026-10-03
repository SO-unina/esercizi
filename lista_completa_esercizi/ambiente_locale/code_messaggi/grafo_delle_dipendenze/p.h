#ifndef _P_H_
#define _P_H_

#include <mqueue.h>

/* Identificativi dei processi, usati nel campo "mittente" dei
 * messaggi di risposta (con le code POSIX non esiste il campo mtype:
 * il mittente e' indicato da un normale campo della struttura). */
#define P1 1
#define P2 2
#define P3 3
#define P4 4
#define P5 5
#define P6 6

/* Nomi delle code POSIX (devono iniziare con '/').
 * Ogni processo che riceve operandi ha una propria coda dedicata:
 * la ricezione selettiva per tipo delle code System V e' sostituita
 * da una coda per destinatario. */
#define QUEUE_OP_P2  "/grafo_op_p2"
#define QUEUE_OP_P3  "/grafo_op_p3"
#define QUEUE_OP_P4  "/grafo_op_p4"
#define QUEUE_OP_P5  "/grafo_op_p5"
#define QUEUE_OP_P6  "/grafo_op_p6"

/* Code dei risultati: una per ogni processo che attende risposte
 * (P1 attende da P2, P3, P4; P3 attende da P5, P6). Il mittente
 * della risposta e' indicato nel campo "mittente" del messaggio. */
#define QUEUE_RIS_P1 "/grafo_ris_p1"
#define QUEUE_RIS_P3 "/grafo_ris_p3"

/* Numero massimo di messaggi accodabili */
#define MAX_MESSAGGI 10

struct msg_operandi {
	int operandi[4];
};

struct msg_risposta {
	int mittente;
	int risposta;
};

#endif // _P_H_
