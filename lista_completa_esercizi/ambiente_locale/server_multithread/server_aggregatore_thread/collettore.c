#include "sensore.h"
#include "collettore.h"

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>

void collettore(mqd_t coda_collettore) {

    printf("Avvio processo collettore (mqd = %d)...\n", (int)coda_collettore);


    for(int i=0; i<10; i++) {

        messaggio msg;
        ssize_t ret;

        printf("Collettore: In attesa di messaggi...\n");

        /* Il buffer di ricezione deve essere grande almeno quanto
         * mq_msgsize della coda, altrimenti mq_receive fallisce
         * con EMSGSIZE. */
        ret = mq_receive(coda_collettore, (char *)&msg, sizeof(messaggio), NULL);

        if(ret < 0) {
            perror("Errore mq_receive");
            exit(1);
        }

        printf("Collettore: Ricevuto valore=%d\n", msg.valore);

    }
}
