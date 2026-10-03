#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>

#include "prodcons_msg.h"
#include "prodcons_server.h"

#define TOTALE_WORKER 3
#define RICHIESTE_PER_WORKER 2

void * worker(void *);

struct param {

    mqd_t coda_richieste;
};

int main() {

    /* Apre (creandola se necessario) la coda delle richieste, con
     * attributi espliciti: il primo processo tra client e server la
     * crea, gli altri la riutilizzano. */
    struct mq_attr attr = {0};
    attr.mq_maxmsg = MAX_MESSAGGI;
    attr.mq_msgsize = sizeof(richiesta_rpc);

    mqd_t coda_richieste = mq_open(CODA_RICHIESTE, O_CREAT | O_RDONLY, 0664, &attr);

    if(coda_richieste == (mqd_t)-1) {
        perror("Errore mq_open");
        exit(1);
    }


    init_monitor();


    pthread_t thread_workers[TOTALE_WORKER];

    for(int i=0; i<TOTALE_WORKER; i++) {

        struct param * p = malloc(sizeof(struct param));
        p->coda_richieste = coda_richieste;

        pthread_create(&thread_workers[i], NULL, worker, p);
    }


    for(int i=0; i<TOTALE_WORKER; i++) {

        pthread_join(thread_workers[i], NULL);
    }

    remove_monitor();

    /* Il server chiude il proprio descrittore; la rimozione della
     * coda (mq_unlink) e' effettuata dal client al termine. */
    mq_close(coda_richieste);

    return 0;
}


void * worker(void * x) {

    struct param * p = (struct param *) x;

    mqd_t coda_richieste = p->coda_richieste;

    ssize_t nrecv;
    int ret;
    int risultato;
    int errore;

    printf("[Worker] In attesa di richieste...\n");


    for(int i=0; i<RICHIESTE_PER_WORKER; i++) {

        richiesta_rpc richiesta;

        /* Il buffer di ricezione deve essere grande almeno quanto
         * mq_msgsize della coda, altrimenti mq_receive fallisce
         * con EMSGSIZE. */
        nrecv = mq_receive(coda_richieste, (char *)&richiesta, sizeof(richiesta_rpc), NULL);

        if(nrecv < 0) {
            perror("Errore mq_receive");
            exit(1);
        }

        /* La funzione da eseguire e' indicata da un normale campo
         * della struttura (non piu' dal campo mtype). */
        if(richiesta.tipo_funzione == TYPE_PRODUCI_CON_SOMMA) {

            int val1 = richiesta.parametro1;
            int val2 = richiesta.parametro2;
            int val3 = richiesta.parametro3;

            printf("[Worker] Ricevuta richiesta di tipo PRODUCI CON SOMMA(%d, %d, %d)\n", val1, val2, val3);

            produci_con_somma(val1, val2, val3);

            risultato = 0;
            errore = 0;

        }
        else if(richiesta.tipo_funzione == TYPE_PRODUCI) {

            int val1 = richiesta.parametro1;

            printf("[Worker] Ricevuta richiesta di tipo PRODUCI(%d)\n", val1);

            produci(val1);

            risultato = 0;
            errore = 0;
        }
        else if(richiesta.tipo_funzione == TYPE_CONSUMA) {

            printf("[Worker] Ricevuta richiesta di tipo CONSUMA(nessun parametro)\n");

            risultato = consuma();
            errore = 0;
        }
        else {

            printf("[Worker] Errore, tipo di richiesta sconosciuta");

            risultato = -1;
            errore = 1;
        }

        int pid_client = richiesta.pid_client;

        risposta_rpc risposta;
        risposta.risultato = risultato;
        risposta.errore = errore;

        /* La risposta viene inviata sulla coda dedicata al client,
         * il cui nome contiene il PID del chiamante. */
        char nome_coda[NOME_CODA_MAX];
        sprintf(nome_coda, CODA_RISPOSTA_FMT, pid_client);

        mqd_t coda_risposta = mq_open(nome_coda, O_WRONLY);

        if(coda_risposta == (mqd_t)-1) {
            perror("Errore mq_open risposta");
            exit(1);
        }

        ret = mq_send(coda_risposta, (const char *)&risposta, sizeof(risposta_rpc), 0);

        if(ret < 0) {
            perror("Errore mq_send");
            exit(1);
        }

        mq_close(coda_risposta);

        printf("[Worker] Inviato risposta: risultato=%d, errore=%d\n", risultato, errore);

    }

    free(p);


    printf("[Worker] Terminazione\n");

    return NULL;
}
