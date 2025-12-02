#include <unistd.h>
#include <sys/wait.h>

int main() {

    pid_t pid;



    pid = fork();

    if(pid == 0) {

        execl("./server", "server", NULL);
    }



    pid = fork();

    if(pid == 0) {

        execl("./client", "client", NULL);
    }


    wait(NULL);
    wait(NULL);
}