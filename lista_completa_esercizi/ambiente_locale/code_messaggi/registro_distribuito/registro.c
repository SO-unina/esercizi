#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

#include "registro.h"

void registro(mqd_t coda_registro_richieste) {

    printf("Registro: Avvio...\n");

    /* Il registro memorizza per ogni server il NOME della
       corrispondente coda di messaggi POSIX (al posto dello ID
       della coda System V). Una stringa vuota indica che il server
       non e' ancora registrato. */

    char code_server[2][QUEUE_NAME_SIZE];
    code_server[0][0] = '\0';
    code_server[1][0] = '\0';

    while(1) {

        messaggio_registro msg_reg;
        int ret;


        printf("Registro: In attesa di messaggi...\n");

        /* Il buffer di ricezione e' grande esattamente mq_msgsize */
        ret = mq_receive(coda_registro_richieste, (char *)&msg_reg, sizeof(messaggio_registro), NULL);

        if(ret < 0) {
            perror("Registro: Errore mq_receive");
            exit(1);
        }


        if(msg_reg.tipo == BIND) {

            int id_server = msg_reg.id_server;

            printf("Registro: Ricevuto messaggio BIND (id_server=%d, coda=%s)\n", id_server, msg_reg.coda);

            if(id_server < 1 || id_server > 2) {
                printf("Registro: ID server non valido\n");
                continue;
            }

            printf("Registro: Registrazione server %d\n", id_server);

            sprintf(code_server[id_server - 1], "%s", msg_reg.coda);

        }
        else if(msg_reg.tipo == QUERY) {

            int id_server = msg_reg.id_server;

            printf("Registro: Ricevuto messaggio QUERY (id_server=%d, coda_risposta=%s)\n", id_server, msg_reg.coda);

            if(id_server < 1 || id_server > 2) {
                printf("Registro: ID server non valido\n");
                continue;
            }

            if(code_server[id_server - 1][0] == '\0') {
                printf("Registro: ID server non registrato\n");
                continue;
            }


            /* La risposta viene inviata sulla coda di risposta
             * dedicata del client, il cui nome e' indicato nel
             * messaggio di QUERY: sostituisce la ricezione selettiva
             * per tipo delle code System V.
             */

            mqd_t coda_risposta = mq_open(msg_reg.coda, O_WRONLY);

            if(coda_risposta == (mqd_t)-1) {
                perror("Registro: Errore mq_open coda risposta");
                continue;
            }

            messaggio_registro msg_risp = {0};
            msg_risp.id_server = id_server;
            sprintf(msg_risp.coda, "%s", code_server[id_server - 1]);

            printf("Registro: Invio messaggio di risposta (id_server=%d, coda=%s)\n", id_server, msg_risp.coda);

            ret = mq_send(coda_risposta, (const char *)&msg_risp, sizeof(messaggio_registro), 0);

            if(ret < 0) {
                perror("Registro: Errore mq_send");
                exit(1);
            }

            mq_close(coda_risposta);
        }
        else if(msg_reg.tipo == EXIT) {

            printf("Registro: Ricevuto messaggio EXIT\n");

            messaggio_server msg_srv;
            msg_srv.tipo = EXIT;
            msg_srv.valore = 0;

            for(int i = 0; i<2; i++) {

                if(code_server[i][0] == '\0') {
                    continue;
                }

                printf("Registro: Invio messaggio EXIT a server %d\n", i);

                mqd_t coda_server = mq_open(code_server[i], O_WRONLY);

                if(coda_server == (mqd_t)-1) {
                    perror("Registro: Errore mq_open coda server");
                    continue;
                }

                ret = mq_send(coda_server, (const char *)&msg_srv, sizeof(messaggio_server), 0);

                if(ret < 0) {
                    perror("Registro: Errore mq_send");
                    exit(1);
                }

                mq_close(coda_server);
            }

            printf("Registro: Uscita\n");

            exit(0);

        }
        else {

            printf("Registro: Ricevuto messaggio non riconosciuto\n");
        }
    }
}
