#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/wait.h>

#include "registro.h"

int main() {


    /* Attributi espliciti della coda delle richieste del registro */
    struct mq_attr attr = {0};
    attr.mq_maxmsg = MAX_MESSAGGI;
    attr.mq_msgsize = sizeof(messaggio_registro);

    /* mq_unlink() preventivo per rimuovere eventuali residui di
       esecuzioni precedenti */
    mq_unlink(QUEUE_REGISTRO);

    mqd_t coda_registro_richieste = mq_open(QUEUE_REGISTRO, O_CREAT | O_RDWR, 0644, &attr);

    if(coda_registro_richieste == (mqd_t)-1) {
        perror("Padre: Errore mq_open");
        exit(1);
    }


    pid_t pid;


    pid = fork();

    if(pid == 0) {
        registro(coda_registro_richieste);
        exit(0);
    }


    for (int i = 0; i < 2; i++)
    {

        pid = fork();

        if (pid == 0)
        {
            int id_server = i+1;

            server(coda_registro_richieste, id_server);

            exit(0);
        }
    }


    /* NOTA: I client sono avviati con un ritardo, per dare il tempo
             ai server di registrarsi sul processo registro.
     */

    sleep(2);

    for (int i = 0; i < 3; i++)
    {

        pid = fork();

        if (pid == 0)
        {
            client(coda_registro_richieste);
            exit(0);
        }
    }



    /* Attesa uscita processi client */
    for(int i = 0; i<3; i++) {
        wait(NULL);
    }



    int ret;
    messaggio_registro msg_reg = {0};

    msg_reg.tipo = EXIT;

    printf("Padre: Invio messaggio EXIT\n");

    ret = mq_send(coda_registro_richieste, (const char *)&msg_reg, sizeof(messaggio_registro), 0);

    if(ret < 0) {
        perror("Padre: Errore mq_send");
        exit(1);
    }

    /* Attesa uscita processi registro e server */
    for(int i = 0; i<3; i++) {
        wait(NULL);
    }


    /* Rimozione della coda del registro: mq_close() chiude il
       descrittore, mq_unlink() rimuove il nome. Il nome sparisce
       subito, la coda viene distrutta quando tutti i processi che la
       avevano aperta la chiudono. Le mailbox dei server e le code di
       risposta dei client sono rimosse dai rispettivi processi. */
    mq_close(coda_registro_richieste);
    mq_unlink(QUEUE_REGISTRO);
}
