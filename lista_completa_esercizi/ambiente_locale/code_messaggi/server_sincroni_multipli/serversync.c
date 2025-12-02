#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "serversync.h"



void send_sinc(int ok_id, int req_id, messaggio * msg)
{

    int ret;

    pid_t pid = getpid();

    request_to_send req_msg;
    ok_to_send ok_msg;


    printf("[%d] Client: invio request-to-send\n", pid);

    req_msg.type = pid;

    ret = msgsnd(req_id, &req_msg, sizeof(request_to_send) - sizeof(long), 0);

    if (ret < 0)
    {
        perror("Errore msgsnd (request-to-send)");
        exit(1);
    }



    printf("[%d] Client: in attesa di ok-to-send...\n", pid);

    ret = msgrcv(ok_id, &ok_msg, sizeof(ok_to_send) - sizeof(long), getpid(), 0);

    if (ret < 0)
    {
        perror("Errore msgrcv (ok-to-send)");
        exit(1);
    }

    long type = ok_msg.type;
    int id_coda = ok_msg.id_coda;

    printf("[%d] Client: ricevuto ok-to-send... type=%ld, id_coda=%d\n", pid, type, id_coda);



    printf("[%d] Client: invio messaggio, coda=%d, type=%ld, valore=%d\n", pid, id_coda, msg->type, msg->val);

    ret = msgsnd(id_coda, msg, sizeof(messaggio) - sizeof(long), 0);

    if (ret < 0)
    {
        perror("Errore msgsnd (coda messaggi)");
        exit(1);
    }

}

void receive_sinc(int msg_id, int ok_id, int req_id, messaggio * msg)
{

    int ret;

    pid_t pid = getpid();

    request_to_send req_msg;
    ok_to_send ok_msg;


    printf("[%d] Server: in attesa di request-to-send...\n", getpid());

    ret = msgrcv(req_id, &req_msg, sizeof(request_to_send) - sizeof(long), 0, 0);

    if (ret < 0)
    {
        perror("Errore msgrcv (request-to-send)");
        exit(1);
    }

    pid_t pid_client = req_msg.type;

    printf("[%d] Server: ricevuto request-to-send, type=%d\n", pid, pid_client);




    long type = req_msg.type;
    int id_coda = msg_id;

    printf("[%d] Server: invio ok-to-send, type=%ld, id_coda=%d\n", pid, type, id_coda);

    ok_msg.type = req_msg.type;
    ok_msg.id_coda = msg_id;

    ret = msgsnd(ok_id, &ok_msg, sizeof(ok_to_send) - sizeof(long), 0);

    if (ret < 0)
    {
        perror("Errore msgsnd (ok-to-send)");
        exit(1);
    }



    printf("[%d] Server: in attesa del messaggio...\n", pid);

    ret = msgrcv(msg_id, msg, sizeof(messaggio) - sizeof(long), 0, 0);

    if (ret < 0)
    {
        perror("Errore msgrcv (coda messaggi)");
        exit(1);
    }

    printf("[%d] Server: ricevuto messaggio, type=%ld, valore=%d\n", pid, msg->type, msg->val);

}
