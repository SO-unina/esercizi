# Lo stesso programma collegato in tre modi diversi:
#   calc_statico        -> libmat.a  (libreria statica), libc dinamica
#   calc_dinamico       -> libmat.so (libreria dinamica), libc dinamica
#   calc_tutto_statico  -> libmat.a e anche la libc statica (-static)
#
#   make              compila tutto con libmat v1 (che ha un bug)
#   make confronto    dimensioni, simboli e dipendenze dei tre eseguibili
#   make aggiorna     sostituisce SOLO libmat.so con la v2 (bug corretto)
#   make esegui       lancia i tre programmi
#   make avvio        tempo di 1000 avvii di ciascuno dei tre programmi
#   make chiamate     tempo di un miliardo di chiamate a una funzione di libreria,
#                     con la libreria statica (ciclo_statico) e dinamica (ciclo_dinamico)

# bash serve per il comando time nelle regole avvio e chiamate
SHELL = /bin/bash
CC = gcc
CFLAGS = -Wall
CURR = $(PWD)
PROGRAMMI = calc_statico calc_dinamico calc_tutto_statico
CICLI = ciclo_statico ciclo_dinamico

.PHONY: all clean confronto aggiorna esegui avvio chiamate

all: $(PROGRAMMI) $(CICLI)

main.o: main.c mat.h
	$(CC) $(CFLAGS) -c main.c

mat_v1.o: mat_v1.c mat.h
	$(CC) $(CFLAGS) -fpic -c mat_v1.c

libmat.a: mat_v1.o
	ar rcs libmat.a mat_v1.o

libmat.so: mat_v1.o
	$(CC) -shared -Wl,-soname,libmat.so -o libmat.so mat_v1.o

# N.B.: se nella cartella ci sono sia libmat.a sia libmat.so, -lmat sceglie
# la libreria DINAMICA. Con -l:libmat.a chiediamo esplicitamente quella statica.
calc_statico: main.o libmat.a
	$(CC) -o $@ main.o -L$(CURR) -l:libmat.a

calc_dinamico: main.o libmat.so
	$(CC) -o $@ main.o -L$(CURR) -lmat -Wl,-rpath,$(CURR)

calc_tutto_statico: main.o libmat.a
	$(CC) -static -o $@ main.o -L$(CURR) -l:libmat.a

confronto: $(PROGRAMMI)
	@echo "=== Dimensioni (byte) ==="
	@ls -l $(PROGRAMMI) | awk '{printf "%10s  %s\n", $$5, $$9}'
	@echo
	@echo "=== Dove si trova 'somma'? (T = dentro l'eseguibile, U = da cercare a runtime) ==="
	@for p in $(PROGRAMMI); do printf "%-20s" $$p; nm $$p | grep -w somma; done
	@echo
	@echo "=== Librerie dinamiche richieste (ldd) ==="
	@for p in $(PROGRAMMI); do echo "$$p:"; ldd ./$$p || true; done

aggiorna: mat_v2.c mat.h
	$(CC) $(CFLAGS) -fpic -c mat_v2.c
	$(CC) -shared -Wl,-soname,libmat.so -o libmat.so mat_v2.o
	@echo ">>> libmat.so aggiornata alla v2. Nessun eseguibile e' stato ricompilato."

# esegui e avvio NON dipendono dai programmi: vogliamo lanciarli cosi' come sono,
# senza che make li ricolleghi dopo "make aggiorna"
esegui:
	@for p in $(PROGRAMMI); do ./$$p; done

avvio:
	@for p in $(PROGRAMMI); do echo "1000 avvii di $$p:"; TIMEFORMAT="  %R secondi"; time (for i in $$(seq 1000); do ./$$p > /dev/null; done); done

# --- Il costo di ogni chiamata ---
# -O2: il ciclo diventa compatto, cosi' nel tempo misurato pesa soprattutto la chiamata
conta.o: conta.c conta.h
	$(CC) $(CFLAGS) -O2 -fpic -c conta.c

libconta.a: conta.o
	ar rcs libconta.a conta.o

libconta.so: conta.o
	$(CC) -shared -Wl,-soname,libconta.so -o libconta.so conta.o

ciclo.o: ciclo.c conta.h
	$(CC) $(CFLAGS) -O2 -c ciclo.c

ciclo_statico: ciclo.o libconta.a
	$(CC) -o $@ ciclo.o -L$(CURR) -l:libconta.a

ciclo_dinamico: ciclo.o libconta.so
	$(CC) -o $@ ciclo.o -L$(CURR) -lconta -Wl,-rpath,$(CURR)

chiamate: $(CICLI)
	@for p in $(CICLI); do echo "$$p:"; TIMEFORMAT="  %R secondi"; time ./$$p; done

clean:
	rm -f *.o libmat.a libmat.so libconta.a libconta.so $(PROGRAMMI) $(CICLI)
