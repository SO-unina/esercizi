#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "serversync.h"



void send_sinc(mqd_t req_id, mqd_t ots_id, const char *nome_coda_risposta, messaggio * msg)
{

    int ret;

    pid_t pid = getpid();

    request_to_send req_msg;
    ok_to_send ok_msg;


    printf("[%d] Client: invio request-to-send (coda_risposta=%s)\n", pid, nome_coda_risposta);

    /* Nel messaggio di richiesta il client indica il nome della
     * propria coda di risposta dedicata (al posto del PID nel campo
     * mtype delle code System V). */
    sprintf(req_msg.coda_risposta, "%s", nome_coda_risposta);

    ret = mq_send(req_id, (const char *)&req_msg, sizeof(request_to_send), 0);

    if (ret < 0)
    {
        perror("Errore mq_send (request-to-send)");
        exit(1);
    }



    printf("[%d] Client: in attesa di ok-to-send...\n", pid);

    /* La risposta arriva sulla coda dedicata del client: non serve
     * (e non esiste) la ricezione selettiva per tipo. Il buffer e'
     * grande esattamente mq_msgsize. */
    ret = mq_receive(ots_id, (char *)&ok_msg, sizeof(ok_to_send), NULL);

    if (ret < 0)
    {
        perror("Errore mq_receive (ok-to-send)");
        exit(1);
    }

    printf("[%d] Client: ricevuto ok-to-send... coda_server=%s\n", pid, ok_msg.coda_server);


    /* Apertura per nome della coda dati del server indicata nello
     * "OK TO SEND" (al posto dello ID di coda System V). */
    mqd_t coda_server = mq_open(ok_msg.coda_server, O_WRONLY);

    if (coda_server == (mqd_t)-1)
    {
        perror("Errore mq_open (coda server)");
        exit(1);
    }

    printf("[%d] Client: invio messaggio, coda=%s, valore=%d\n", pid, ok_msg.coda_server, msg->val);

    ret = mq_send(coda_server, (const char *)msg, sizeof(messaggio), 0);

    if (ret < 0)
    {
        perror("Errore mq_send (coda messaggi)");
        exit(1);
    }

    mq_close(coda_server);

}

void receive_sinc(mqd_t msg_id, const char *nome_coda_server, mqd_t req_id, messaggio * msg)
{

    int ret;

    pid_t pid = getpid();

    request_to_send req_msg;
    ok_to_send ok_msg;


    printf("[%d] Server: in attesa di request-to-send...\n", getpid());

    ret = mq_receive(req_id, (char *)&req_msg, sizeof(request_to_send), NULL);

    if (ret < 0)
    {
        perror("Errore mq_receive (request-to-send)");
        exit(1);
    }

    printf("[%d] Server: ricevuto request-to-send, coda_risposta=%s\n", pid, req_msg.coda_risposta);



    /* Lo "OK TO SEND" viene inviato sulla coda di risposta dedicata
     * indicata dal client, e contiene il nome della coda dati del
     * server. */

    printf("[%d] Server: invio ok-to-send, coda_server=%s\n", pid, nome_coda_server);

    sprintf(ok_msg.coda_server, "%s", nome_coda_server);

    mqd_t coda_risposta = mq_open(req_msg.coda_risposta, O_WRONLY);

    if (coda_risposta == (mqd_t)-1)
    {
        perror("Errore mq_open (coda risposta client)");
        exit(1);
    }

    ret = mq_send(coda_risposta, (const char *)&ok_msg, sizeof(ok_to_send), 0);

    if (ret < 0)
    {
        perror("Errore mq_send (ok-to-send)");
        exit(1);
    }

    mq_close(coda_risposta);



    printf("[%d] Server: in attesa del messaggio...\n", pid);

    ret = mq_receive(msg_id, (char *)msg, sizeof(messaggio), NULL);

    if (ret < 0)
    {
        perror("Errore mq_receive (coda messaggi)");
        exit(1);
    }

    printf("[%d] Server: ricevuto messaggio, pid=%d, valore=%d\n", pid, msg->pid, msg->val);

}
