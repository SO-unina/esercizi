# SO-ES3-Makefile-Librerie-GDB

In questa lezione esercitativa sono affrontati i **Makefiles**, lo sviluppo di **librerie statiche** e **librerie dinamiche** e l'utilizzo del debugger **GDB**.

Sommario degli esempi:

- [**0_pipeline_gcc**](0_pipeline_gcc): Le fasi della compilazione (preprocessore, compilatore, assemblatore, linker);
- [**1_makefile_esempio**](1_makefile_esempio): Utilizzo di base dei Makefile;
- [**2_elfdumping**](2_elfdumping): Ottenere il dump di un file oggetto; simboli e rilocazioni;
- [**3_statlib**](3_statlib): Sviluppo libreria statica;
- [**4_dynlib**](4_dynlib): Sviluppo libreria dinamica;
- [**4b_statica_vs_dinamica**](4b_statica_vs_dinamica): Confronto tra collegamento statico e dinamico: dimensione degli eseguibili, aggiornamento di una libreria senza ricompilare, tempo di avvio e costo delle chiamate;
- [**5_gdb**](5_gdb): Utilizzo del debugger;
