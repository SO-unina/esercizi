#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>

#include "registro.h"

void server(mqd_t coda_registro_richieste, int id_server) {

    int risorsa = 0;

    int ret;

    /* Il server crea una propria mailbox, il cui nome e' costruito
       a partire dal PID (sostituisce la coda privata System V) */

    char nome_coda[QUEUE_NAME_SIZE];
    sprintf(nome_coda, QUEUE_SRV_PREFIX "%d", getpid());

    struct mq_attr attr = {0};
    attr.mq_maxmsg = MAX_MESSAGGI;
    attr.mq_msgsize = sizeof(messaggio_server);

    mq_unlink(nome_coda);

    mqd_t coda_server = mq_open(nome_coda, O_CREAT | O_RDONLY, 0644, &attr);

    if(coda_server == (mqd_t)-1) {
        perror("Server: Errore mq_open");
        exit(1);
    }

    printf("Server: Invio messaggio BIND (id_server=%d, coda=%s)\n", id_server, nome_coda);

    messaggio_registro msg_reg = {0};
    msg_reg.tipo = BIND;
    msg_reg.id_server = id_server;

    /* Il nome della mailbox viene comunicato al registro nel
       messaggio di "bind" (al posto dello ID della coda System V) */
    sprintf(msg_reg.coda, "%s", nome_coda);

    ret = mq_send(coda_registro_richieste, (const char *)&msg_reg, sizeof(messaggio_registro), 0);

    if(ret < 0) {
        perror("Server: Errore mq_send");
        exit(1);
    }


    while(1) {

        printf("Server: RISORSA = %d\n", risorsa);

        printf("Server: In attesa di messaggi...\n");

        messaggio_server msg_srv;

        /* Il buffer di ricezione e' grande esattamente mq_msgsize */
        ret = mq_receive(coda_server, (char *)&msg_srv, sizeof(messaggio_server), NULL);

        if(ret < 0) {
            perror("Server: Errore mq_receive");
            exit(1);
        }


        /* Il tipo di messaggio (SERVICE/EXIT) e' un normale campo
           della struttura */

        if(msg_srv.tipo == SERVICE) {

            int valore = msg_srv.valore;

            printf("Server: Ricevuto messaggio SERVICE (id_server=%d, valore=%d)\n", id_server, valore);

            risorsa = msg_srv.valore;
        }
        else if(msg_srv.tipo == EXIT) {

            printf("Server: Ricevuto messaggio EXIT (id_server=%d)\n", id_server);

            /* Rimozione della mailbox: mq_close() chiude il
               descrittore, mq_unlink() rimuove il nome (il nome
               sparisce subito, la coda quando tutti chiudono) */
            mq_close(coda_server);
            mq_unlink(nome_coda);

            printf("Server: Uscita\n");

            exit(0);
        }
        else {

            printf("Server: Ricevuto messaggio non riconosciuto\n");
        }
    }
}
