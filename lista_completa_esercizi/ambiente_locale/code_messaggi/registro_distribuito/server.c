#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "registro.h"

void server(int id_coda_registro_richieste, int id_coda_registro_risposte, int id_server) {

    int risorsa = 0;

    int ret;

    int id_coda_server = msgget(IPC_PRIVATE, IPC_CREAT | 0644);

    if(id_coda_server < 0) {
        perror("Server: Errore msgget");
        exit(1);
    }

    printf("Server: Invio messaggio BIND (id_server=%d, id_coda=%d)\n", id_server, id_coda_server);

    messaggio_registro msg_reg;
    msg_reg.tipo = BIND;
    msg_reg.id_server = id_server;
    msg_reg.id_coda = id_coda_server;

    ret = msgsnd(id_coda_registro_richieste, &msg_reg, sizeof(messaggio_registro) - sizeof(long), 0);

    if(ret < 0) {
        perror("Server: Errore msgsnd");
        exit(1);
    }


    while(1) {

        printf("Server: RISORSA = %d\n", risorsa);

        printf("Server: In attesa di messaggi...\n");

        messaggio_server msg_srv;

        ret = msgrcv(id_coda_server, &msg_srv, sizeof(messaggio_server) - sizeof(long), 0, 0);

        if(ret < 0) {
            perror("Server: Errore msgrcv");
            exit(1);
        }


        if(msg_srv.tipo == SERVICE) {

            int valore = msg_srv.valore;

            printf("Server: Ricevuto messaggio SERVICE (id_server=%d, valore=%d)\n", id_server, valore);

            risorsa = msg_srv.valore;
        }
        else if(msg_srv.tipo == EXIT) {

            printf("Server: Ricevuto messaggio EXIT (id_server=%d)\n", id_server);

            msgctl(id_coda_server, IPC_RMID, 0);

            printf("Server: Uscita\n");

            exit(0);
        }
        else {

            printf("Server: Ricevuto messaggio non riconosciuto\n");
        }
    }
}
