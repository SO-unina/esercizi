#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

void StackInit(Stack * s, int dim) {

	pthread_mutex_init( &(s->m), NULL );
	pthread_cond_init( &(s->ok_push), NULL );
	pthread_cond_init( &(s->ok_pop), NULL );

	s->dati = (Elem *) malloc(sizeof(Elem)*dim);

	s->dim = dim;
	s->testa = 0;
}


void StackRemove(Stack * s) {

	pthread_mutex_destroy( &(s->m) );
	pthread_cond_destroy( &(s->ok_push) );
	pthread_cond_destroy( &(s->ok_pop) );

	free(s->dati);
}

void StackPush(Stack * s, Elem e) {

	pthread_mutex_lock( &(s->m) );

	while( s->dim==s->testa )
		pthread_cond_wait( &(s->ok_push), &(s->m) );

	s->dati[s->testa] = e;
	s->testa++;

	printf("Inserimento: %d\n", e);

	pthread_cond_signal( &(s->ok_pop) );

	pthread_mutex_unlock( &(s->m) );
}


Elem StackPop(Stack * s) {

	int elemento;

	pthread_mutex_lock( &(s->m) );

	while( s->testa==0 )
		pthread_cond_wait( &(s->ok_pop), &(s->m) );

	s->testa--;
	elemento=s->dati[s->testa];

	printf("Prelievo: %d\n", elemento);

	pthread_cond_signal( &(s->ok_push) );

	pthread_mutex_unlock( &(s->m) );

	return elemento;
}

int StackSize(Stack * s) {

	int size;

	/* Anche se è solo una lettura senza modifiche,
	 * è comunque necessario l'ingresso nel monitor,
	 * per evitare race condition con altri metodi che
	 * modificano la variabile.
	 *
	 * Il metodo StackSize() non può essere richiamato da
	 * StackPush() o StackPop() se tutti acquisiscono lo
	 * stesso mutex, per non incorrere in un deadlock.
	 *
	 * Per consentire ad un thread di acquisire un mutex
	 * più di una volta, è necessario inizializzare il mutex
	 * con l'attributo PTHREAD_MUTEX_RECURSIVE.
	 */

	pthread_mutex_lock( &(s->m) );

	size = s->testa;

	pthread_mutex_unlock( &(s->m) );

	return size;
}
