#include "sensore.h"
#include "aggregatore.h"
#include "collettore.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {


    int id_coda_sensore = msgget(IPC_PRIVATE, IPC_CREAT | 0644 );

    if(id_coda_sensore < 0) {
        perror("Errore msgget");
        exit(1);
    }


    int id_code_collettori[3];

    for(int i=0; i<3; i++) {

        id_code_collettori[i] = msgget(IPC_PRIVATE, IPC_CREAT | 0644 );

        if(id_code_collettori[i] < 0) {
            perror("Errore msgget");
            exit(1);
        }
    }


    pid_t pid;


    pid = fork();

    if(pid == 0) {
        sensore(id_coda_sensore);
        exit(0);
    }



    pid = fork();

    if(pid == 0) {
        aggregatore(id_coda_sensore, id_code_collettori);
        exit(0);
    }



    for(int i=0; i<3; i++) {

        pid = fork();

        if(pid == 0) {
            collettore(id_code_collettori[i]);
            exit(0);
        }
    }


    for(int i=0; i<5; i++) {
        wait(NULL);
    }



    msgctl(id_coda_sensore, IPC_RMID, 0);

    for(int i=0; i<3; i++) {
        msgctl(id_code_collettori[i], IPC_RMID, 0);
    }
}