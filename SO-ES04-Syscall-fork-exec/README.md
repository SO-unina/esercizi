# SO-ES4-Syscall-fork-exec

In questa lezione esercitativa sono affrontate le system call utilizzate per la gestione dei processi. In particolare, andremo a trattare system call per la **creazione** e **cancellazione**, il **segnalamento**, e il **controllo** di un processo.

Sommario degli esempi:

- [**1_getpid**](1_getpid): Utilizzo delle system call ``getpid()`` e ``getppid()``; osservare i processi con ``ps``, ``pstree`` e ``/proc``;
- [**2_fork**](2_fork): Creazione di processi con ``fork()``: memoria separata, creazione di *N* figli (soluzione corretta e sbagliata);
- [**2b_wait_exit**](2b_wait_exit): Attesa e terminazione con ``wait()`` ed ``exit()``: stato di terminazione, processi zombie e orfani;
- [**3_exec**](3_exec): Utilizzo della famiglia di system call ``exec()``; le system call eseguite davvero, viste con ``strace``;
- [**3b_copy_on_write**](3b_copy_on_write): Quanto costa una ``fork()``: il copy-on-write misurato;
- [**4_shell**](4_shell): Implementazione di una shell.
