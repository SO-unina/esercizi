#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "serversync.h"

int main()
{

    /* Coda "OK TO SEND" */

    key_t ok_key = ftok(".", 'o');

    int ok_id = msgget(ok_key, 0644);

    if (ok_id < 0)
    {
        perror("Errore creazione coda ok-to-send");
        exit(1);
    }

    /* Coda "REQUEST TO SEND" */

    key_t req_key = ftok(".", 'r');

    int req_id = msgget(req_key, 0644);

    if (req_id < 0)
    {
        perror("Errore creazione coda request-to-send");
        exit(1);
    }




    srand(getpid());

    for (int i = 0; i < 2; i++)
    {
        messaggio msg;

        msg.type = getpid();
        msg.val = rand() % 100;

        printf("[%d] Client: invio val=%d\n", getpid(), msg.val);

        /* NOTA: la coda a cui inviare il messaggio sarà comunicata dal
         *       processo server che prende carico della "REQUEST TO SEND"
         */

        send_sinc(ok_id, req_id, &msg);

        sleep(2);
    }

    return 0;
}