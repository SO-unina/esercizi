#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	int pid, st;
	
	pid=fork(); 
		
	if (pid==-1){
		fprintf(stderr, "Generazione del processo fallita\n");
		return -1;
	}	
	if  (pid==0){
                printf("ESECUZIONE del comando 'ls -l'...\n");
		execl("/bin/ls", "ls", "-l", NULL);
		fprintf(stderr,"Exec fallita!!\n");
		_exit(1); //questa versione della exit esegue subito la system call exit_group senza le operazioni di housekeeping preliminari della exit classica
	}
	else if (pid>0){
		wait(&st);
		printf("Il figlio %d ha terminato l'esecuzione ", pid);
		if ((char)st==0)
			printf("con stato: %d\n\n",st>>8);
		else
			printf("involontariamente!\n\n");
	}		 
return 0;
}
