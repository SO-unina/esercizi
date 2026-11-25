# -pthread: con SIGEV_THREAD la notifica viene gestita in un thread.
# Da glibc 2.34 le funzioni dei pthread e le mq_* sono in libc, e le due
# opzioni non servono piu'; non fanno danni (e servono con versioni piu' vecchie).

all: start

start: notifica.c
	gcc -o start notifica.c -lrt -pthread

clean:
	rm -f start
