#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prodcons_msg.h"
#include "prodcons_server.h"

#define TOTALE_WORKER 3
#define RICHIESTE_PER_WORKER 2

void * worker(void *);

struct param {

    int id_coda_richieste;
    int id_coda_risposte;
};

int main() {

    key_t chiave_coda_richieste = ftok(".", 'a');

    int id_coda_richieste = msgget(chiave_coda_richieste, IPC_CREAT | 0664);

    if(id_coda_richieste < 0) {
        perror("Errore msgget");
        exit(1);
    }


    key_t chiave_coda_risposte = ftok(".", 'b');

    int id_coda_risposte = msgget(chiave_coda_risposte, IPC_CREAT | 0664);

    if(id_coda_risposte < 0) {
        perror("Errore msgget");
        exit(1);
    }


    init_monitor();


    pthread_t thread_workers[TOTALE_WORKER];

    for(int i=0; i<TOTALE_WORKER; i++) {

        struct param * p = malloc(sizeof(struct param));
        p->id_coda_richieste = id_coda_richieste;
        p->id_coda_risposte = id_coda_risposte;

        pthread_create(&thread_workers[i], NULL, worker, p);
    }


    for(int i=0; i<TOTALE_WORKER; i++) {

        pthread_join(thread_workers[i], NULL);
    }

    remove_monitor();

    return 0;
}


void * worker(void * x) {

    struct param * p = (struct param *) x;

    int id_coda_richieste = p->id_coda_richieste;
    int id_coda_risposte = p->id_coda_risposte;

    int ret;
    int risultato;
    int errore;

    printf("[Worker] In attesa di richieste...\n");


    for(int i=0; i<RICHIESTE_PER_WORKER; i++) {

        richiesta_rpc richiesta;

        ret = msgrcv(id_coda_richieste, &richiesta, sizeof(richiesta_rpc) - sizeof(long), 0, 0);

        if(ret < 0) {
            perror("Errore msgrcv");
            exit(1);
        }

        
        if(richiesta.type == TYPE_PRODUCI_CON_SOMMA) {

            int val1 = richiesta.parametro1;
            int val2 = richiesta.parametro2;
            int val3 = richiesta.parametro3;

            printf("[Worker] Ricevuta richiesta di tipo PRODUCI CON SOMMA(%d, %d, %d)\n", val1, val2, val3);

            produci_con_somma(val1, val2, val3);

            risultato = 0;
            errore = 0;

        }
        else if(richiesta.type == TYPE_PRODUCI) {

            int val1 = richiesta.parametro1;

            printf("[Worker] Ricevuta richiesta di tipo PRODUCI(%d)\n", val1);

            produci(val1);

            risultato = 0;
            errore = 0;
        }
        else if(richiesta.type == TYPE_CONSUMA) {

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
        risposta.type = pid_client;
        risposta.risultato = risultato;
        risposta.errore = errore;

        ret = msgsnd(id_coda_risposte, &risposta, sizeof(risposta_rpc) - sizeof(long), 0);

        if(ret < 0) {
            perror("Errore msgsnd");
            exit(1);
        }

        printf("[Worker] Inviato risposta: risultato=%d, errore=%d\n", risultato, errore);

    }

    free(p);


    printf("[Worker] Terminazione\n");

    return NULL;
}
