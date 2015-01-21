CC=gcc
#CC=clang # enable if you want (and have clang installed)
CFLAGS=-O3 -std=c99

all: library testtool

testtool: library
	$(CC) $(CFLAGS) -c testtool.c
	$(CC) -L . -o testtool testtool.o -ldbticket

library: .modules
	$(CC) $(CFLAGS) -c -fpic libmain.c
	$(CC) -shared -o libdbticket libmain.o mod_*.o intf_*.o

.modules:
	$(CC) $(CFLAGS) -c -fpic mod_*.c intf_*.c

libdarwin: library
	mv libdbticket libdbticket.dylib

libunix: library
	mv libdbticket libdbticket.so

clean:
	rm testtool libdbticket
	rm *.o