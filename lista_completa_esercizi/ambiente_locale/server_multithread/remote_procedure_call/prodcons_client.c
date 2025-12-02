#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prodcons_msg.h"
#include "prodcons_client.h"

static int id_coda_richieste;
static int id_coda_risposte;

void init_client(int id_coda_richieste_parametro, int id_coda_risposte_parametro) {

    id_coda_richieste = id_coda_richieste_parametro;
    id_coda_risposte = id_coda_risposte_parametro;
}

void produci_con_somma(int val1, int val2, int val3) {

    richiesta_rpc richiesta;

    richiesta.type = TYPE_PRODUCI_CON_SOMMA;
    richiesta.parametro1 = val1;
    richiesta.parametro2 = val2;
    richiesta.parametro3 = val3;
    richiesta.pid_client = getpid();

    int ret = msgsnd(id_coda_richieste, &richiesta, sizeof(richiesta_rpc) - sizeof(long), 0);

    if(ret < 0) {
        perror("Errore msgsnd");
        exit(1);
    }

    printf("[Client] Invio richiesta PRODUCI_CON_SOMMA(%d, %d, %d)\n", val1, val2, val3);


    risposta_rpc risposta;

    ret = msgrcv(id_coda_risposte, &risposta, sizeof(risposta_rpc) - sizeof(long), getpid(), 0);

    if(ret < 0) {
        perror("Errore msgrcv");
        exit(1);
    }

    int risultato = risposta.risultato;
    int errore = risposta.errore;

    printf("[Client] Ricevuto risposta: risultato=%d, errore=%d\n", risultato, errore);
}

void produci(int val) {

    richiesta_rpc richiesta;

    richiesta.type = TYPE_PRODUCI;
    richiesta.parametro1 = val;
    richiesta.pid_client = getpid();

    int ret = msgsnd(id_coda_richieste, &richiesta, sizeof(richiesta_rpc) - sizeof(long), 0);

    if(ret < 0) {
        perror("Errore msgsnd");
        exit(1);
    }

    printf("[Client] Invio richiesta PRODUCI(%d)\n", val);


    risposta_rpc risposta;

    ret = msgrcv(id_coda_risposte, &risposta, sizeof(risposta_rpc) - sizeof(long), getpid(), 0);

    if(ret < 0) {
        perror("Errore msgrcv");
        exit(1);
    }

    int risultato = risposta.risultato;
    int errore = risposta.errore;

    printf("[Client] Ricevuto risposta: risultato=%d, errore=%d\n", risultato, errore);
}

int consuma() {

    richiesta_rpc richiesta;

    richiesta.type = TYPE_CONSUMA;
    richiesta.pid_client = getpid();

    int ret = msgsnd(id_coda_richieste, &richiesta, sizeof(richiesta_rpc) - sizeof(long), 0);

    if(ret < 0) {
        perror("Errore msgsnd");
        exit(1);
    }

    printf("[Client] Invio richiesta CONSUMA(nessun parametro)\n");


    risposta_rpc risposta;

    ret = msgrcv(id_coda_risposte, &risposta, sizeof(risposta_rpc) - sizeof(long), getpid(), 0);

    if(ret < 0) {
        perror("Errore msgrcv");
        exit(1);
    }

    int risultato = risposta.risultato;
    int errore = risposta.errore;

    printf("[Client] Ricevuto risposta: risultato=%d, errore=%d\n", risultato, errore);

    return risultato;
}