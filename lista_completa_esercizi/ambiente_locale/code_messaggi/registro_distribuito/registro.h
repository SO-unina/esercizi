#ifndef _REGISTRO_H_
#define _REGISTRO_H_

#include <mqueue.h>

/* Nome della coda POSIX su cui il registro riceve le richieste
 * ("bind", "query", "exit"). I nomi delle code POSIX iniziano con '/'. */
#define QUEUE_REGISTRO "/registro_richieste"

/* Prefissi per i nomi delle code create dinamicamente:
 * - ogni server crea una propria mailbox "/registro_srv_<pid>";
 * - ogni client crea una propria coda di risposta dedicata
 *   "/registro_risp_<pid>", il cui nome viene comunicato al registro
 *   nel messaggio di "query". Con le code POSIX non esiste la
 *   ricezione selettiva per tipo: la risposta arriva sulla coda
 *   dedicata del client. */
#define QUEUE_SRV_PREFIX  "/registro_srv_"
#define QUEUE_RISP_PREFIX "/registro_risp_"

#define QUEUE_NAME_SIZE 64

/* Numero massimo di messaggi accodabili */
#define MAX_MESSAGGI 10

/* Messaggio da/verso il registro. Il campo "tipo" (BIND, QUERY, EXIT)
 * e' un normale campo della struttura (non piu' il campo mtype).
 * Il campo "coda" contiene:
 * - in un messaggio BIND: il nome della mailbox del server;
 * - in un messaggio QUERY: il nome della coda di risposta del client;
 * - in un messaggio di risposta: il nome della mailbox del server
 *   richiesto. */
typedef struct {
    int tipo;
    int id_server;
    char coda[QUEUE_NAME_SIZE];
} messaggio_registro;

typedef struct {
    int tipo;
    int valore;
} messaggio_server;

#define BIND 1
#define QUERY 2
#define SERVICE 3
#define EXIT 4


void client(mqd_t coda_registro_richieste);
void registro(mqd_t coda_registro_richieste);
void server(mqd_t coda_registro_richieste, int id_server);

#endif
