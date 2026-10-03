#include "sensore.h"

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <time.h>
#include <unistd.h>

void sensore(mqd_t coda_sensore) {

    printf("Avvio processo sensore...\n");


    srand(time(NULL));

    for(int i=0; i<10; i++) {

        messaggio msg;

        msg.valore = rand() % 11;

        printf("Sensore: Invio valore=%d\n", msg.valore);

        int ret = mq_send(coda_sensore, (const char *)&msg, sizeof(messaggio), 0);

        if(ret < 0) {
            perror("Errore mq_send");
            exit(1);
        }

        sleep(1);
    }
}
