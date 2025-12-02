#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

#include "registro.h"

int main() {


    int id_coda_registro_richieste = msgget(IPC_PRIVATE, IPC_CREAT | 0644);

    if(id_coda_registro_richieste < 0) {
        perror("Padre: Errore msgget");
        exit(1);
    }


    int id_coda_registro_risposte = msgget(IPC_PRIVATE, IPC_CREAT | 0644);

    if(id_coda_registro_risposte < 0) {
        perror("Padre: Errore msgget");
        exit(1);
    }


    pid_t pid;


    pid = fork();

    if(pid == 0) {
        registro(id_coda_registro_richieste,id_coda_registro_risposte);
        exit(0);
    }


    for (int i = 0; i < 2; i++)
    {
        pid = fork();

        if (pid == 0)
        {
            int id_server = i+1;

            server(id_coda_registro_richieste, id_coda_registro_risposte, id_server);

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
            client(id_coda_registro_richieste, id_coda_registro_risposte);
            exit(0);
        }
    }



    /* Attesa uscita processi client */
    for(int i = 0; i<3; i++) {
        wait(NULL);
    }



    int ret;
    messaggio_registro msg_reg;

    msg_reg.tipo = EXIT;

    printf("Padre: Invio messaggio EXIT\n");

    ret = msgsnd(id_coda_registro_richieste, &msg_reg, sizeof(messaggio_registro) - sizeof(long), 0);

    if(ret < 0) {
        perror("Padre: Errore msgsnd");
        exit(1);
    }

    /* Attesa uscita processi registro e server */
    for(int i = 0; i<3; i++) {
        wait(NULL);
    }


    msgctl(id_coda_registro_richieste, IPC_RMID, 0);
    msgctl(id_coda_registro_risposte, IPC_RMID, 0);
}
