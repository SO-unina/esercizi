all: lett_scrit_starv_entrambi

lett_scrit_starv_entrambi: lett_scrit_starv_entrambi.o procedure.o
	gcc -o lettore_scrittore_exe lett_scrit_starv_entrambi.o procedure.o -lrt -lpthread

lett_scrit_starv_entrambi.o: lett_scrit_starv_entrambi.c header.h
	gcc -c lett_scrit_starv_entrambi.c

procedure.o: procedure.c header.h
	gcc -c procedure.c

clean :
	 rm -f *.o
	 rm -f lettore_scrittore_exe
