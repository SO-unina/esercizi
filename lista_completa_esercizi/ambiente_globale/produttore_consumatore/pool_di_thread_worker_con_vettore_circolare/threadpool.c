#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAXTASKS 10
#define NUMTHREADS 5

typedef struct {
	int operand_1;
	int operand_2;
} Task;

typedef struct {
	Task tasklist[MAXTASKS];
	pthread_t pool[NUMTHREADS];
	int head;
	int tail;
	pthread_mutex_t mutex;
	pthread_cond_t task_full;
	pthread_cond_t task_empty;
} ThreadPool;

void ThreadPoolInit(ThreadPool * t);
void ThreadPoolPutTask(ThreadPool * t, Task task);
void ThreadPoolTerminate(ThreadPool * t);
void * ThreadPoolExecTask(void * t);

void ThreadPoolInit(ThreadPool * t) {
	t->head = 0;
	t->tail = 1;

	pthread_mutex_init(&t->mutex, NULL);
	pthread_cond_init(&t->task_full, NULL);
	pthread_cond_init(&t->task_empty, NULL);
	
	int i;
	int rc;
	
	for(i=0; i<NUMTHREADS; i++) {
		printf("Creating thread %d\n", i);
		rc = pthread_create(&t->pool[i], NULL, ThreadPoolExecTask, (void *)t);
		if(rc) {
			printf("ERROR pthread_create(): %d\n", rc);
			exit(-1);
		}
	}
}

void ThreadPoolPutTask(ThreadPool * t, Task task) {
	pthread_mutex_lock(&t->mutex);
	
	while( t->tail == t->head ) {
		pthread_cond_wait(&t->task_full, &t->mutex);
	}

	t->tasklist[t->tail].operand_1 = task.operand_1;
	t->tasklist[t->tail].operand_2 = task.operand_2;

	t->tail = (t->tail + 1) % MAXTASKS;
	
	pthread_cond_signal(&t->task_empty);

	pthread_mutex_unlock(&t->mutex);
}

void ThreadPoolTerminate(ThreadPool * t) {

	int i;
	int rc;
	
	for(i=0; i<NUMTHREADS; i++) {
		printf("Destroying thread %d\n", i);
		rc = pthread_cancel(t->pool[i]);
		if(rc) {
			printf("ERROR pthread_cancel(): %d\n", rc);
			exit(-1);
		}
	}
	
	pthread_mutex_destroy(&t->mutex);
	pthread_cond_destroy(&t->task_full);
	pthread_cond_destroy(&t->task_empty);
}

void * ThreadPoolExecTask(void * p) {

	ThreadPool * t = (ThreadPool *)p;
	Task task;

	while(1) {

		pthread_mutex_lock(&t->mutex);
	
		while( t->tail == ((t->head + 1) % MAXTASKS) ) {
			pthread_cond_wait(&t->task_empty, &t->mutex);
		}

		t->head = (t->head + 1) % MAXTASKS;

		task.operand_1 = t->tasklist[t->head].operand_1;
		task.operand_2 = t->tasklist[t->head].operand_2;
		
		pthread_cond_signal(&t->task_full);

		pthread_mutex_unlock(&t->mutex);
		
		printf("Thread %lu: %d + %d = %d\n", pthread_self(), task.operand_1, task.operand_2, task.operand_1+task.operand_2);
		
		sleep(1);

	}
}

int main() {

	ThreadPool t;
	Task task;
	int i;
	
	ThreadPoolInit(&t);
	
	for(i=0; i<20; i++) {
		task.operand_1 = i;
		task.operand_2 = i;
		
		ThreadPoolPutTask(&t, task); 
	}

	sleep(10);
	
	ThreadPoolTerminate(&t);

	return 0;
}

