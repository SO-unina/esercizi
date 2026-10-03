#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS     5


void * PrintHello(void * arg) { // start routine 

	int *p_arg = (int*) arg;
   	printf("\n%d: Hello World! (arg address: %p)\n", *p_arg, p_arg);
   	free(p_arg);

	pthread_exit(NULL); // terminazione thread "figlio"

}

int main (int argc, char *argv[]) {

	pthread_t threads[NUM_THREADS];
	
	int rc;
	for(int i=0; i<NUM_THREADS; i++){
		printf("Creating thread %d\n", i);

		/* Allocazione dell'argomento da passare al thread tramite HEAP */

		int *ptr_i = malloc(sizeof(int));
      		*ptr_i = i;

		rc = pthread_create(&threads[i], NULL, PrintHello, (void *) ptr_i);
		if (rc!=0){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(NULL); // terminazione del thread "padre"
}

