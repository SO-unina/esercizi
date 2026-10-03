#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>

#include "registro.h"

void client(mqd_t coda_registro_richieste) {

    printf("Client: Avvio...\n");

    srand(getpid());


    int ret;
    int id_server = (rand() % 2) + 1;


    /* Il client crea una propria coda di risposta dedicata, il cui
       nome e' costruito a partire dal PID. Sostituisce la ricezione
       selettiva per tipo delle code System V. */

    char nome_risposta[QUEUE_NAME_SIZE];
    sprintf(nome_risposta, QUEUE_RISP_PREFIX "%d", getpid());

    struct mq_attr attr = {0};
    attr.mq_maxmsg = MAX_MESSAGGI;
    attr.mq_msgsize = sizeof(messaggio_registro);

    mq_unlink(nome_risposta);

    mqd_t coda_risposta = mq_open(nome_risposta, O_CREAT | O_RDONLY, 0644, &attr);

    if(coda_risposta == (mqd_t)-1) {
        perror("Client: Errore mq_open coda risposta");
        exit(1);
    }


    messaggio_registro msg_reg = {0};
    msg_reg.tipo = QUERY;
    msg_reg.id_server = id_server;

    /* Il nome della coda di risposta viene comunicato al registro
       nel messaggio di richiesta */
    sprintf(msg_reg.coda, "%s", nome_risposta);

    printf("Client: Invio messaggio QUERY (id_server=%d, coda_risposta=%s)\n", id_server, nome_risposta);

    ret = mq_send(coda_registro_richieste, (const char *)&msg_reg, sizeof(messaggio_registro), 0);

    if(ret < 0) {
        perror("Client: Errore mq_send");
        exit(1);
    }

    printf("Client: Attesa messaggio di risposta dal registro...\n");

    messaggio_registro msg_risp;

    /* Il buffer di ricezione e' grande esattamente mq_msgsize:
       con un buffer piu' piccolo mq_receive fallisce con EMSGSIZE */
    ret = mq_receive(coda_risposta, (char *)&msg_risp, sizeof(messaggio_registro), NULL);

    if(ret < 0) {
        perror("Client: Errore mq_receive");
        exit(1);
    }


    printf("Client: Ricevuto messaggio di risposta dal registro (id_server=%d, coda=%s)\n", id_server, msg_risp.coda);


    /* Apertura per nome della mailbox del server indicata dal registro */

    mqd_t coda_server = mq_open(msg_risp.coda, O_WRONLY);

    if(coda_server == (mqd_t)-1) {
        perror("Client: Errore mq_open coda server");
        exit(1);
    }


    for(int i = 0; i<3; i++) {

        int valore = rand() % 11;

        messaggio_server msg_srv;

        msg_srv.tipo = SERVICE;
        msg_srv.valore = valore;

        printf("Client: Invio messaggio SERVICE (id_server=%d, coda=%s, valore=%d)\n", id_server, msg_risp.coda, valore);

        ret = mq_send(coda_server, (const char *)&msg_srv, sizeof(messaggio_server), 0);

        if(ret < 0) {
            perror("Client: Errore mq_send");
            exit(1);
        }

        sleep(1);
    }

    /* Rimozione della coda di risposta dedicata: il nome sparisce
       subito, la coda quando tutti i processi la chiudono */
    mq_close(coda_server);
    mq_close(coda_risposta);
    mq_unlink(nome_risposta);

    printf("Client: Uscita\n");
}
