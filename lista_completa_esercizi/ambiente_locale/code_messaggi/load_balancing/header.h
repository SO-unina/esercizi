#ifndef __HEADER__
#define __HEADER__

#include <sys/types.h>
#include <mqueue.h>

#define TOTALE_SERVER 3
#define TOTALE_CLIENT 2
#define TOTALE_MESSAGGI 6

/* Nomi delle code POSIX (devono iniziare con '/'):
 * una coda per le richieste dei client verso il balancer, e una coda
 * dedicata per ogni server. Il nome della coda dell'i-esimo server e'
 * costruito come QUEUE_SERVER_PREFIX seguito dal numero del server. */
#define QUEUE_BALANCER      "/lb_balancer"
#define QUEUE_SERVER_PREFIX "/lb_server_"
#define QUEUE_NAME_SIZE 32

/* Numero massimo di messaggi accodabili */
#define MAX_MESSAGGI_CODA 10

/* Struct del messaggio.
 * Nota: con le code POSIX non esiste il campo "tipo" (mtype) delle
 * code System V; nella soluzione originale il campo era comunque
 * inutilizzato, per cui il messaggio contiene solo il PID. */
struct messaggio {
	pid_t PID;
};


void Client(mqd_t msg_id_balancer);
void Balancer(mqd_t msg_id_balancer, mqd_t msg_id_server[]);
void Server(mqd_t msg_id_server);


#endif /* __HEADER__ */
