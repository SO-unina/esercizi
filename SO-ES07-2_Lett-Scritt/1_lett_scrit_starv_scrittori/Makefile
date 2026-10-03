all: lett_scrit_starv_scrittori

lett_scrit_starv_scrittori: lett_scrit_starv_scrittori.o procedure.o
	gcc -o lettore_scrittore_exe lett_scrit_starv_scrittori.o procedure.o -lrt -lpthread

lett_scrit_starv_scrittori.o: lett_scrit_starv_scrittori.c header.h
	gcc -c lett_scrit_starv_scrittori.c

procedure.o: procedure.c header.h
	gcc -c procedure.c

clean:
	rm -f *.o
	rm -f lettore_scrittore_exe
