// Conto alla rovescia per il decollo... che non arriva mai.
// Lanciate il programma e "salvatelo" attaccandovi con gdb.
#include <stdio.h>
#include <unistd.h>

int main(void){
	int secondi = 9;

	printf("Conto alla rovescia (PID %d)\n", getpid());

	while(secondi != 0){
		printf("%d... ", secondi);
		fflush(stdout);
		secondi -= 2;		// 9, 7, 5, 3, 1, -1, -3, ... non vale mai 0!
		sleep(1);
	}

	printf("\nDecollo!\n");
	return 0;
}
