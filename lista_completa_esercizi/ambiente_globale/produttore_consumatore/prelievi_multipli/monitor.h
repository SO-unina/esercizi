/***PROTOTIPI DELLE PROCEDURE PER LA REALIZZAZIONE DEL COSTRUTTO MONITOR***/

/* Costrutto Monitor realizzato con i Pthreads (mutex + condition
 * variables), con semantica SIGNAL-AND-CONTINUE.
 *
 * NOTA DIDATTICA: con la semantica signal-and-continue il thread
 * risvegliato da una signal_condition non rientra immediatamente nel
 * monitor: riacquisisce il mutex solo quando il thread segnalante lo
 * rilascia. Nel frattempo la condizione logica potrebbe essere cambiata,
 * quindi la wait_condition va SEMPRE invocata all'interno di un ciclo
 * while che ricontrolla la condizione logica.
 */

#ifndef __MONITOR_H
#define __MONITOR_H

#include <pthread.h>

typedef struct {

  /* mutex del monitor */
  pthread_mutex_t mutex;

  /* numero di variabili condition */
  int num_var_cond;

  /* array delle condition variables pthread */
  pthread_cond_t *conds;

  /* array dei contatori dei thread in attesa su ogni condition */
  int *cond_counts;

} Monitor;

/* monitor e numero di variabili condition */
void init_monitor (Monitor*, int);
void enter_monitor(Monitor*);
void leave_monitor(Monitor*);
void remove_monitor(Monitor*);
void wait_condition(Monitor*, int);
void signal_condition(Monitor*, int);
int queue_condition(Monitor*, int);

#endif
