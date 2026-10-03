#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "serversync.h"

int main()
{

    /* Coda "REQUEST TO SEND", gia' creata dal processo principale */

    mqd_t req_id = mq_open(QUEUE_RTS, O_RDONLY);

    if (req_id == (mqd_t)-1)
    {
        perror("Errore apertura coda request-to-send");
        exit(1);
    }

    /* Creazione di una coda di messaggi privata del processo server,
     * con nome costruito a partire dal PID (sostituisce la coda
     * privata System V; il nome viene comunicato ai client nello
     * "OK TO SEND"). */

    char nome_coda[QUEUE_NAME_SIZE];
    sprintf(nome_coda, QUEUE_SRV_PREFIX "%d", getpid());

    struct mq_attr attr = {0};
    attr.mq_maxmsg = MAX_MESSAGGI;
    attr.mq_msgsize = sizeof(messaggio);

    mq_unlink(nome_coda);

    mqd_t msg_id = mq_open(nome_coda, O_CREAT | O_RDONLY, 0644, &attr);

    if (msg_id == (mqd_t)-1)
    {
        perror("Errore creazione coda messaggi");
        exit(1);
    }

    for (int i = 0; i < 4; i++)
    {
        messaggio msg;

        receive_sinc(msg_id, nome_coda, req_id, &msg);

        printf("[%d] Server: ricevuto val=%d\n", getpid(), msg.val);
    }

    /* Rimozione della coda privata: mq_unlink() fa sparire subito il
     * nome, ma la coda viene distrutta solo quando tutti i processi
     * che la avevano aperta la chiudono. Lo scambio e' sincrono
     * (l'ultimo messaggio e' gia' stato ricevuto), quindi non serve
     * attendere i client prima di rimuoverla. */
    mq_close(msg_id);
    mq_unlink(nome_coda);

    mq_close(req_id);

    return 0;
}
