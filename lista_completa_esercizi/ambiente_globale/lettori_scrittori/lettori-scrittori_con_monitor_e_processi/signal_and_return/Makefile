all: meteo

meteo: meteo.o procedure.o monitor_signal_return.o
	gcc -o meteo meteo.o procedure.o monitor_signal_return.o

meteo.o: meteo.c header.h
	gcc -c -o meteo.o meteo.c

procedure.o: procedure.c header.h
	gcc -c -o procedure.o procedure.c

monitor_signal_return.o: monitor_signal_return.c monitor_signal_return.h
	gcc -c -o monitor_signal_return.o monitor_signal_return.c

clean:
	rm -rf *.o
	rm -rf meteo
