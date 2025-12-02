#ifndef _SERVERSYNC_H_
#define _SERVERSYNC_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
    long type;
} request_to_send;

typedef struct {
    long type;
    int id_coda;
} ok_to_send;

typedef struct {
    long type;
    int val;
} messaggio;

void receive_sinc(int msg_id, int ok_id, int req_id, messaggio * msg);
void send_sinc(int ok_id, int req_id, messaggio * msg);

#endif