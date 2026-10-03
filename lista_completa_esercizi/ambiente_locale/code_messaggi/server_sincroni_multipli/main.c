#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

#include "serversync.h"

int main()
{

    /* Coda "REQUEST TO SEND", condivisa tra tutti i client e i
     * server. Le risposte "OK TO SEND" viaggiano invece sulle code
     * di risposta dedicate create dai singoli client. */

    struct mq_attr attr = {0};
    attr.mq_maxmsg = MAX_MESSAGGI;
    attr.mq_msgsize = sizeof(request_to_send);

    /* mq_unlink() preventivo per rimuovere eventuali residui di
     * esecuzioni precedenti */
    mq_unlink(QUEUE_RTS);

    mqd_t req_id = mq_open(QUEUE_RTS, O_CREAT | O_RDWR, 0644, &attr);

    if (req_id == (mqd_t)-1)
    {
        perror("Errore creazione coda request-to-send");
        exit(1);
    }


    for (int i = 0; i < 2; i++)
    {
        printf("[%d] Avvio server\n", getpid());

        pid_t pid = fork();

        if (pid == 0)
        {
            /* Processo Server */
            execl("./server", "server", NULL);

            perror("Errore exec server");
            exit(1);
        }
    }


    for (int i = 0; i < 4; i++)
    {
        printf("[%d] Avvio client\n", getpid());

        pid_t pid = fork();

        if (pid == 0)
        {
            /* Processo Client */
            execl("./client", "client", NULL);

            perror("Errore exec client");
            exit(1);
        }
    }


    printf("[%d] In attesa di terminazione...\n", getpid());

    for (int i = 0; i < 6; i++)
    {
        wait(NULL);
    }

    printf("[%d] Deallocazione code\n", getpid());

    /* mq_close() chiude il descrittore, mq_unlink() rimuove il nome:
     * il nome sparisce subito, la coda viene distrutta quando tutti i
     * processi che la avevano aperta la chiudono. */
    mq_close(req_id);
    mq_unlink(QUEUE_RTS);

    return 0;
}
