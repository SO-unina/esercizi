#include "sensore.h"
#include "aggregatore.h"
#include "collettore.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    /* Attributi espliciti delle code: tutti i messaggi hanno la
     * stessa dimensione. */
    struct mq_attr attr = {0};
    attr.mq_maxmsg = MAX_MESSAGGI;
    attr.mq_msgsize = sizeof(messaggio);

    /* Rimozione preventiva di eventuali residui di esecuzioni precedenti */
    mq_unlink(CODA_SENSORE);

    mqd_t coda_sensore = mq_open(CODA_SENSORE, O_CREAT | O_RDWR, 0644, &attr);

    if(coda_sensore == (mqd_t)-1) {
        perror("Errore mq_open");
        exit(1);
    }


    char nome_coda[NOME_CODA_MAX];
    mqd_t code_collettori[3];

    for(int i=0; i<3; i++) {

        sprintf(nome_coda, CODA_COLLETTORE_FMT, i);

        mq_unlink(nome_coda);

        code_collettori[i] = mq_open(nome_coda, O_CREAT | O_RDWR, 0644, &attr);

        if(code_collettori[i] == (mqd_t)-1) {
            perror("Errore mq_open");
            exit(1);
        }
    }

    /* I descrittori delle code vengono ereditati dai figli con la fork */

    pid_t pid;


    pid = fork();

    if(pid == 0) {
        sensore(coda_sensore);
        exit(0);
    }



    pid = fork();

    if(pid == 0) {
        aggregatore(coda_sensore, code_collettori);
        exit(0);
    }



    for(int i=0; i<3; i++) {

        pid = fork();

        if(pid == 0) {
            collettore(code_collettori[i]);
            exit(0);
        }
    }


    for(int i=0; i<5; i++) {
        wait(NULL);
    }


    /* Chiusura e rimozione delle code */

    mq_close(coda_sensore);
    mq_unlink(CODA_SENSORE);

    for(int i=0; i<3; i++) {
        mq_close(code_collettori[i]);
        sprintf(nome_coda, CODA_COLLETTORE_FMT, i);
        mq_unlink(nome_coda);
    }
}
