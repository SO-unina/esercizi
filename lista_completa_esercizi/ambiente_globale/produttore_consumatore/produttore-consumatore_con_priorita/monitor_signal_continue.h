/***PROTOTIPI DELLE PROCEDURE PER LA REALIZZAZIONE DEL COSTRUTTO MONITOR***/
#ifndef __MONITOR_H
#define __MONITOR_H

typedef struct {

//id del semaforo per realizzare il mutex del monitor
  int mutex;

//numero di variabili condition
  int num_var_cond;

//id del gruppo sem associati alle var.cond
  int id_conds;

//id della memoria condivisa per i contatori delle variabili condition
  int id_shared;

//array delle variabili condition_count
  int *cond_counts;

} Monitor;

//monitor e numero di variabili condition
void init_monitor (Monitor*, int); 
void enter_monitor(Monitor*);
void leave_monitor(Monitor*);
void remove_monitor(Monitor*);
void wait_condition(Monitor*,int);
void signal_condition(Monitor*,int);
int queue_condition(Monitor*,int); 


#endif
