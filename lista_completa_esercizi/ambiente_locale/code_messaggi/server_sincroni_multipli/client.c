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

    mqd_t req_id = mq_open(QUEUE_RTS, O_WRONLY);

    if (req_id == (mqd_t)-1)
    {
        perror("Errore apertura coda request-to-send");
        exit(1);
    }


    /* Il client crea una propria coda di risposta dedicata per lo
     * "OK TO SEND", con nome costruito a partire dal PID: sostituisce
     * la ricezione selettiva con mtype = PID delle code System V. */

    char nome_ots[QUEUE_NAME_SIZE];
    sprintf(nome_ots, QUEUE_OTS_PREFIX "%d", getpid());

    struct mq_attr attr = {0};
    attr.mq_maxmsg = MAX_MESSAGGI;
    attr.mq_msgsize = sizeof(ok_to_send);

    mq_unlink(nome_ots);

    mqd_t ots_id = mq_open(nome_ots, O_CREAT | O_RDONLY, 0644, &attr);

    if (ots_id == (mqd_t)-1)
    {
        perror("Errore creazione coda ok-to-send");
        exit(1);
    }


    srand(getpid());

    for (int i = 0; i < 2; i++)
    {
        messaggio msg;

        msg.pid = getpid();
        msg.val = rand() % 100;

        printf("[%d] Client: invio val=%d\n", getpid(), msg.val);

        /* NOTA: la coda a cui inviare il messaggio sarà comunicata dal
         *       processo server che prende carico della "REQUEST TO SEND"
         */

        send_sinc(req_id, ots_id, nome_ots, &msg);

        sleep(2);
    }

    /* Rimozione della coda di risposta dedicata: il nome sparisce
     * subito, la coda quando tutti i processi la chiudono. */
    mq_close(ots_id);
    mq_unlink(nome_ots);

    mq_close(req_id);

    return 0;
}
