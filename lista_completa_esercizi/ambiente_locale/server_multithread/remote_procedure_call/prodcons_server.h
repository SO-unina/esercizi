#ifndef PRODCONS_SERVER
#define PRODCONS_SERVER

#include <pthread.h>

typedef struct {

    pthread_mutex_t mutex;
    pthread_cond_t cv_prod;
    pthread_cond_t cv_cons;

    int stato;  // OCCUPATO = 0, LIBERO = 1

    int buffer;

} monitor_prodcons;

#define OCCUPATO 0
#define LIBERO 1

void init_monitor();
void remove_monitor();

#endif