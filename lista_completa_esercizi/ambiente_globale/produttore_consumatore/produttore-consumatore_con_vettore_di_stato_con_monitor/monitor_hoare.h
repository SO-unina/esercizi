/***PROTOTIPI DELLE PROCEDURE PER LA REALIZZAZIONE DEL COSTRUTTO MONITOR***/


typedef struct {

//id del semaforo per realizzare il mutex del monitor
  int mutex;

//id del semaforo per realizzare la coda urgent
  int urgent_sem;

//numero di variabili condition
  int num_var_cond;

//id del gruppo sem associati alle var.cond
  int id_conds;

//id della memoria condivisa per i contatori delle variabili condition e della coda urgent
  int id_shared;

//array delle variabili condition_count
  int *cond_counts;

//contatore del numero di processi sospesi sulla coda urgent
  int *urgent_count;

} Monitor;

//monitor e numero di variabili condition
void init_monitor (Monitor*, int); 
void enter_monitor(Monitor*);
void leave_monitor(Monitor*);
void remove_monitor(Monitor*);
void wait_condition(Monitor*,int);
void signal_condition(Monitor*,int);
int queue_condition(Monitor*,int); 



