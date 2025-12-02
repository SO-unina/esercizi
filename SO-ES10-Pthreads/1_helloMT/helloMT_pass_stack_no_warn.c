#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS     5

void * PrintHello(void * arg) { // start routine 

	int *p_arg = (int*) arg;
   	printf("\n%d: Hello World! (arg address: %p)\n", *p_arg, p_arg);
	pthread_exit(NULL); // terminazione thread "figlio"

}

int main (int argc, char *argv[]) {

	pthread_t threads[NUM_THREADS];
	
	int rc;
	for(int i=0; i<NUM_THREADS; i++){
		printf("Creating thread %d\n", i);
		
		/* Allocazione dell'argomento da passare al thread tramite stack del thread padre.
                        Questo tipo di allocazione non genera warning dal punto di vista del typecast
			perchè passo correttamente un riferimento della variabile i.
                        Ma andrò incontro a comporatmenti non definiti perchè la variabile i "vive" nello stack
			del padre, ed è soggetta a tutta la vita (visibilità e accessi) del padre stesso.
			Questo tipo di passaggio dei parametri porta ad esecuzioni errate!
                */

		rc = pthread_create(&threads[i], NULL, PrintHello, (void *) &i);
		if (rc!=0){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(NULL); // terminazione del thread "padre"
}

