#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

asmlinkage long sys_mysyscall(pid_t pid, char * buffer, unsigned int buf_size) {

        struct task_struct * mytask;
        char * nome_processo;

        printk(KERN_DEBUG "System call mysyscall trova-processo\n");

	/*
          ESTRATTO struttura task_struct ritornata dalla funzione find_task_by_vpid
	  * executable name, excluding path.
	  *
	  * - normally initialized setup_new_exec()
	  * - access it with [gs]et_task_comm()
	  * - lock it with task_lock()
	  */
        // char    comm[TASK_COMM_LEN];

        mytask = find_task_by_vpid(pid);	 // ricerca del process control block, in base al PID

        if(!mytask) {
                printk(KERN_DEBUG "Processo non trovato: %d\n", pid);	      // find_task_by_vpid è fallita
                return -ESRCH;
        }

        nome_processo = mytask->comm;
        printk(KERN_DEBUG "Processo trovato: %d = %s\n", pid, nome_processo);

        if( copy_to_user( buffer, nome_processo, (buf_size<TASK_COMM_LEN ? buf_size : TASK_COMM_LEN)) ) {

                printk(KERN_DEBUG "Errore di accesso alla memoria: %p\n", buffer);	// copy_to_user è fallita
                return -EFAULT;
        }

        return 0;
}
