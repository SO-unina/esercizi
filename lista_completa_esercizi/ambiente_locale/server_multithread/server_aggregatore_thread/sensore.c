#include "sensore.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>

void sensore(int id_coda_sensore) {

    printf("Avvio processo sensore...\n");


    srand(time(NULL));

    for(int i=0; i<10; i++) {

        messaggio msg;

        msg.tipo = TIPO;

        msg.valore = rand() % 11;

        printf("Sensore: Invio valore=%d\n", msg.valore);

        int ret = msgsnd(id_coda_sensore, &msg, sizeof(messaggio) - sizeof(long), 0);

        if(ret < 0) {
            perror("Errore msgsnd");
            exit(1);
        }
        
        sleep(1);
    }
}