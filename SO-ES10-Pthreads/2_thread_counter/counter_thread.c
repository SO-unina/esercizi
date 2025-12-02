#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS     2

struct counter {
    pthread_mutex_t mutex;
    int valore;
};

void *Counter(void * x);  // incrementa il valore 100000 volte

int main (int argc, char *argv[]) {
	pthread_t threads[NUM_THREADS];

	struct counter * p = malloc(sizeof(struct counter));

	p->valore = 0;
	pthread_mutex_init(&p->mutex, NULL);

	for(int i=0; i<NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, Counter, (void *)p);

	for(int i=0; i<NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	printf("Valore del contatore = %d\n", p->valore);
}


void *Counter(void * x) {
	struct counter * p = x;

	for(int i=0; i<100000; i++) {
		// provare ad eliminare pthread_mutex_lock/pthread_mutex_unlock per ottenere una race condition!
		pthread_mutex_lock(&p->mutex);
		p->valore++;
		pthread_mutex_unlock(&p->mutex);
	}
	pthread_exit(NULL);
}
