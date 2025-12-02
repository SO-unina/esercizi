#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{

    /* Coda "OK TO SEND" */

    key_t ok_key = ftok(".", 'o');

    int ok_id = msgget(ok_key, IPC_CREAT | 0644);

    if (ok_id < 0)
    {
        perror("Errore creazione coda messaggi");
        exit(1);
    }

    /* Coda "REQUEST TO SEND" */

    key_t req_key = ftok(".", 'r');

    int req_id = msgget(req_key, IPC_CREAT | 0644);

    if (req_id < 0)
    {
        perror("Errore creazione coda messaggi");
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

    msgctl(ok_id, IPC_RMID, NULL);
    msgctl(req_id, IPC_RMID, NULL);

    return 0;
}