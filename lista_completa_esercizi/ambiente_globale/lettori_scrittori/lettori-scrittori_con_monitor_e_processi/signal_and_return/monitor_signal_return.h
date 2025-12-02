/***PROTOTIPI DELLE PROCEDURE PER LA REALIZZAZIONE DEL COSTRUTTO MONITOR***/


typedef struct {
  int mutex;
  int num_var_cond;
//id del gruppo sem associati alle var.cond
  int id_conds;     
//array delle variabili condition_count
   int *cond_counts; 
//identificativo memoria condivisa
   int id_shared;    
} Monitor;

//monitor e numero di variabili condition
void init_monitor (Monitor*, int); 
void enter_monitor(Monitor*);
void leave_monitor(Monitor*);
void remove_monitor(Monitor*);
void wait_condition(Monitor*,int);
void signal_condition(Monitor*,int);
int queue_condition(Monitor*,int); 



