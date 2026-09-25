# Ogni fase della compilazione è una regola: il Makefile ricalca
# esattamente la catena editor -> preprocessore -> compilatore -> linker

.PHONY: all clean macchina

all: saluti

# 1. Preprocessore: espande #include, #define, #ifdef  (testo C -> testo C)
saluti.i: saluti.c
	gcc -E saluti.c -o saluti.i

# 2. Compilatore: traduce il C in assembly  (testo C -> testo assembly)
saluti.s: saluti.i
	gcc -S saluti.i -o saluti.s

# 3. Assemblatore: traduce l'assembly in codice oggetto  (-> binario ELF rilocabile)
saluti.o: saluti.s
	gcc -c saluti.s -o saluti.o

# 4. Linker: collega il codice oggetto con la libreria C  (-> eseguibile ELF)
saluti: saluti.o
	gcc saluti.o -o saluti

# Compilazione condizionale: stesso sorgente, programma diverso
macchina: saluti.c
	gcc -DMACCHINA saluti.c -o saluti_macchina
	./saluti_macchina

clean:
	rm -f saluti saluti_macchina *.i *.s *.o
