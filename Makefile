CC=gcc
CFLAGS=-O3

all: testtool

testtool:
	$(CC) -o testtool testtool.c mod_*.c

dynamic:
	$(CC) $(CFLAGS) -c -fpic libmain.c mod_*.c
	$(CC) -o libdbticket.so libmain.o mod_*.o 
	rm *.o


clean:
	rm testtool
	rm db-ticket.so
	rm db-ticket.a