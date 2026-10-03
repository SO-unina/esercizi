#ifndef _SERVERSYNC_H_
#define _SERVERSYNC_H_

#include <sys/types.h>
#include <mqueue.h>

/* Nome della coda POSIX condivisa per le richieste "REQUEST TO SEND"
 * (da client a server). I nomi delle code POSIX iniziano con '/'. */
#define QUEUE_RTS "/ssm_request_to_send"

/* Prefissi per i nomi delle code create dinamicamente:
 * - ogni client crea una propria coda di risposta dedicata
 *   "/ssm_ots_<pid>" su cui riceve lo "OK TO SEND" (sostituisce la
 *   ricezione selettiva con mtype = PID delle code System V);
 * - ogni server crea una propria coda dati "/ssm_srv_<pid>". */
#define QUEUE_OTS_PREFIX "/ssm_ots_"
#define QUEUE_SRV_PREFIX "/ssm_srv_"

#define QUEUE_NAME_SIZE 64

/* Numero massimo di messaggi accodabili */
#define MAX_MESSAGGI 10

/* "REQUEST TO SEND": il client indica il nome della propria coda di
 * risposta dedicata (al posto del PID nel campo mtype). */
typedef struct {
    char coda_risposta[QUEUE_NAME_SIZE];
} request_to_send;

/* "OK TO SEND": il server indica il nome della propria coda dati
 * (al posto dello ID della coda System V). */
typedef struct {
    char coda_server[QUEUE_NAME_SIZE];
} ok_to_send;

/* Messaggio dati: PID del client mittente e valore casuale. */
typedef struct {
    pid_t pid;
    int val;
} messaggio;

void receive_sinc(mqd_t msg_id, const char *nome_coda_server, mqd_t req_id, messaggio * msg);
void send_sinc(mqd_t req_id, mqd_t ots_id, const char *nome_coda_risposta, messaggio * msg);

#endif
