#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "registro.h"

void registro(int id_coda_registro_richieste, int id_coda_registro_risposte) {

    printf("Registro: Avvio...\n");

    int id_code_server[2];
    id_code_server[0] = 0;
    id_code_server[1] = 0;

    while(1) {

        messaggio_registro msg_reg;
        int ret;


        printf("Registro: In attesa di messaggi...\n");

        ret = msgrcv(id_coda_registro_richieste, &msg_reg, sizeof(messaggio_registro) - sizeof(long), 0, 0);

        if(ret < 0) {
            perror("Registro: Errore msgrcv");
            exit(1);
        }


        if(msg_reg.tipo == BIND) {

            int id_server = msg_reg.id_server;
            int id_coda = msg_reg.id_coda;

            printf("Registro: Ricevuto messaggio BIND (id_server=%d, id_coda=%d)\n", id_server, id_coda);

            if(id_server < 1 || id_server > 2) {
                printf("Registro: ID server non valido\n");
                continue;
            }

            printf("Registro: Registrazione server %d\n", id_server);

            id_code_server[id_server - 1] = id_coda;

        }
        else if(msg_reg.tipo == QUERY) {

            int id_server = msg_reg.id_server;

            printf("Registro: Ricevuto messaggio QUERY (id_server=%d)\n", id_server);

            if(id_server < 1 || id_server > 2) {
                printf("Registro: ID server non valido\n");
                continue;
            }

            int id_coda = id_code_server[id_server - 1];

            if(id_coda == 0) {
                printf("Registro: ID server non registrato\n");
                continue;
            }


            /* Nel messaggio di risposta, si indica lo ID del server nel campo "tipo",
             * per consentire la ricezione selettiva da parte del client.
             */

            messaggio_registro msg_risp;
            msg_risp.tipo = id_server;
            msg_risp.id_coda = id_coda;

            printf("Registro: Invio messaggio di risposta (id_server=%d, id_coda=%d)\n", id_server, id_coda);

            ret = msgsnd(id_coda_registro_risposte, &msg_risp, sizeof(messaggio_registro) - sizeof(long), 0);

            if(ret < 0) {
                perror("Registro: Errore msgsnd");
                exit(1);
            }
        }
        else if(msg_reg.tipo == EXIT) {

            printf("Registro: Ricevuto messaggio EXIT\n");

            messaggio_server msg_srv;
            msg_srv.tipo = EXIT;

            for(int i = 0; i<2; i++) {

                if(id_code_server[i] == 0) {
                    continue;
                }

                printf("Registro: Invio messaggio EXIT a server %d\n", i);

                ret = msgsnd(id_code_server[i], &msg_srv, sizeof(messaggio_server) - sizeof(long), 0);

                if(ret < 0) {
                    perror("Registro: Errore msgsnd");
                    exit(1);
                }
            }

            printf("Registro: Uscita\n");

            exit(0);

        }
        else {

            printf("Registro: Ricevuto messaggio non riconosciuto\n");
        }
    }
}
