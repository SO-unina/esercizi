#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main() {
	int pid, st;
	const int NMAX=50;		// numero massimo di parametri
	const int CMAX=1000;	// numero massimo di caratteri letti dalla shell
	char *argv[NMAX];		// array dei parametri da fornire ad execvp 
	char comLine[CMAX+1];	// contiene i comandi forniti da tastiera
	char *token;			// variabile di supporto per estrarre il comando e i parametri da comLine
	char endShell[5]="quit"; // stringa da digitare per uscire dalla shell

	int n=0;


	
	printf("\n     ^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^\n");
	printf("\n     --    Questo programma simula il funzionamento di una shell    --\n");
	printf("\n     ^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^\n");
	printf("\n                        * Per uscire digita quit *                    \n"); 


	while (1){

		printf("\nmy-shell>> ");

		// legge una riga dall'utente;
		// esce dalla shell in caso di errore
		if( fgets(comLine, CMAX+1, stdin) == NULL ) {
			fprintf(stderr, "Errore nella lettura degli input dell'utente\n");
			return -1;
		}

		// si rimuove il ritorno a capo presente alla fine
		// della riga letta dall'utente
		// (esempio: "ls\n\0" viene trasformato in "ls\0\0")	
		char * p;
		if( (p=strrchr(comLine,'\n')) != NULL ) {
			*p='\0';
		}

		// esce dalla shell se l'utente scrive il comando "quit"
		if( strcmp(comLine,endShell) == 0 ) {
			printf("\n                           * Arrivederci!! *                    \n\n"); 
			return 0;
		}


		// si analizza la stringa di ingresso dell'utente
		n=0;
		token=strtok(comLine," ");		
		while(token != NULL){
			argv[n]=token;
			n++;
			token=strtok (NULL, " ");		
		}
		argv[n]=(char *)0;     // ultimo argomento della execv deve puntare a null.
		

		// se l'utente non ha digitato alcun comando
		// (ad esempio ha solo premuto invio) si passa
		// alla iterazione successiva
		if(argv[0] == NULL) {
			continue;
		}


		pid=fork();		
		if (pid==-1){
			fprintf(stderr, "Generazione del processo fallita\n");	
			return -1;
		}
		if  (pid==0){
			
			if (strcmp(argv[n-1],"&")==0){  // esecuzione in background
				
				int pid_nipote = fork();
				if (pid_nipote==-1){
					fprintf(stderr, "Generazione del processo nipote fallita\n");	
					return -1;
				}
				if (pid_nipote==0){
					int fd = open ("/dev/null", O_RDONLY);  // redirezione dell'input
					dup2(fd,0);	// redirige "stdin" verso "/dev/null"
					close(fd);
					argv[n-1]=(char *)0;  // metto "null" al posto del "&"
					execvp(argv[0],argv); // eseguo il comando in background (nel nipote)
					fprintf(stderr,"%s: comando non trovato!\n",argv[0]);
				 	_exit(1);
				}
				else if (pid_nipote>0)
					// il figlio termina senza attendere la terminazione del nipote
					_exit(0);		

			} else {
				execvp(argv[0],argv);  // eseguo il comando in foreground
				fprintf(stderr,"%s: comando non trovato!\n",argv[0]);
				_exit(1);
			}
				
		}
		else if (pid>0){
			wait(&st);

			if (WIFEXITED(st) && WEXITSTATUS(st)!=0)
				printf("Il figlio ha terminato con stato di errore (%d)\n", WEXITSTATUS(st));
			
		}

	}


}
	 	

