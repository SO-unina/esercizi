#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>

#include "prodcons_msg.h"

int main() {

    pid_t pid;

    /* Rimozione preventiva di eventuali residui di esecuzioni
     * precedenti, prima di lanciare client e server. */
    mq_unlink(CODA_RICHIESTE);


    pid = fork();

    if(pid == 0) {

        execl("./server", "server", (char *)NULL);
    }



    pid = fork();

    if(pid == 0) {

        execl("./client", "client", (char *)NULL);
    }


    wait(NULL);
    wait(NULL);
}
