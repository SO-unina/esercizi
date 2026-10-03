#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>

#include "prodcons_msg.h"
#include "prodcons_client.h"

static mqd_t coda_richieste;
static mqd_t coda_risposte;
static char nome_coda_risposte[NOME_CODA_MAX];

void init_client(void) {

    /* Apre (creandola se necessario) la coda delle richieste: il primo
     * processo tra client e server la crea, gli altri la riutilizzano. */
    struct mq_attr attr_richieste = {0};
    attr_richieste.mq_maxmsg = MAX_MESSAGGI;
    attr_richieste.mq_msgsize = sizeof(richiesta_rpc);

    coda_richieste = mq_open(CODA_RICHIESTE, O_CREAT | O_WRONLY, 0664, &attr_richieste);

    if(coda_richieste == (mqd_t)-1) {
        perror("Errore mq_open richieste");
        exit(1);
    }

    /* Crea la coda di risposta dedicata a questo processo, il cui nome
     * contiene il PID: sostituisce la ricezione selettiva con mtype=PID. */
    sprintf(nome_coda_risposte, CODA_RISPOSTA_FMT, getpid());

    struct mq_attr attr_risposte = {0};
    attr_risposte.mq_maxmsg = MAX_MESSAGGI;
    attr_risposte.mq_msgsize = sizeof(risposta_rpc);

    mq_unlink(nome_coda_risposte);

    coda_risposte = mq_open(nome_coda_risposte, O_CREAT | O_RDONLY, 0664, &attr_risposte);

    if(coda_risposte == (mqd_t)-1) {
        perror("Errore mq_open risposte");
        exit(1);
    }
}

void fini_client(void) {

    mq_close(coda_richieste);

    mq_close(coda_risposte);
    mq_unlink(nome_coda_risposte);
}

void produci_con_somma(int val1, int val2, int val3) {

    richiesta_rpc richiesta;

    richiesta.tipo_funzione = TYPE_PRODUCI_CON_SOMMA;
    richiesta.parametro1 = val1;
    richiesta.parametro2 = val2;
    richiesta.parametro3 = val3;
    richiesta.pid_client = getpid();

    int ret = mq_send(coda_richieste, (const char *)&richiesta, sizeof(richiesta_rpc), 0);

    if(ret < 0) {
        perror("Errore mq_send");
        exit(1);
    }

    printf("[Client] Invio richiesta PRODUCI_CON_SOMMA(%d, %d, %d)\n", val1, val2, val3);


    risposta_rpc risposta;

    ssize_t nrecv = mq_receive(coda_risposte, (char *)&risposta, sizeof(risposta_rpc), NULL);

    if(nrecv < 0) {
        perror("Errore mq_receive");
        exit(1);
    }

    int risultato = risposta.risultato;
    int errore = risposta.errore;

    printf("[Client] Ricevuto risposta: risultato=%d, errore=%d\n", risultato, errore);
}

void produci(int val) {

    richiesta_rpc richiesta;

    richiesta.tipo_funzione = TYPE_PRODUCI;
    richiesta.parametro1 = val;
    richiesta.pid_client = getpid();

    int ret = mq_send(coda_richieste, (const char *)&richiesta, sizeof(richiesta_rpc), 0);

    if(ret < 0) {
        perror("Errore mq_send");
        exit(1);
    }

    printf("[Client] Invio richiesta PRODUCI(%d)\n", val);


    risposta_rpc risposta;

    ssize_t nrecv = mq_receive(coda_risposte, (char *)&risposta, sizeof(risposta_rpc), NULL);

    if(nrecv < 0) {
        perror("Errore mq_receive");
        exit(1);
    }

    int risultato = risposta.risultato;
    int errore = risposta.errore;

    printf("[Client] Ricevuto risposta: risultato=%d, errore=%d\n", risultato, errore);
}

int consuma() {

    richiesta_rpc richiesta;

    richiesta.tipo_funzione = TYPE_CONSUMA;
    richiesta.pid_client = getpid();

    int ret = mq_send(coda_richieste, (const char *)&richiesta, sizeof(richiesta_rpc), 0);

    if(ret < 0) {
        perror("Errore mq_send");
        exit(1);
    }

    printf("[Client] Invio richiesta CONSUMA(nessun parametro)\n");


    risposta_rpc risposta;

    ssize_t nrecv = mq_receive(coda_risposte, (char *)&risposta, sizeof(risposta_rpc), NULL);

    if(nrecv < 0) {
        perror("Errore mq_receive");
        exit(1);
    }

    int risultato = risposta.risultato;
    int errore = risposta.errore;

    printf("[Client] Ricevuto risposta: risultato=%d, errore=%d\n", risultato, errore);

    return risultato;
}
