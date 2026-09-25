# -g   : inserisce nell'eseguibile le informazioni di debug (righe, variabili, tipi)
# -O0  : nessuna ottimizzazione, cosi' il codice macchina segue fedelmente il sorgente
CC = gcc
CFLAGS = -g -O0 -Wall
PROGRAMMI = media_voti crash cassaforte bloccato

.PHONY: all clean

all: $(PROGRAMMI)

# Regola implicita: ogni programma si ottiene dal .c con lo stesso nome
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGRAMMI) core core.*
