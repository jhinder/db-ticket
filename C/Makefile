CC=gcc
# Note: compiling with gcc will include libgcc as a dynamic library.
# You can use clang if you don't want that.
#CC=clang
CFLAGS=-O3 -std=c99

all: library testtool

testtool: library
	$(CC) $(CFLAGS) -c testtool.c
	$(CC) -L . -o testtool testtool.o -ldbticket

library: .modules .inflate
	$(CC) $(CFLAGS) -c -fpic libmain.c
	$(CC) -shared -o libdbticket libmain.o tinfl.o mod_*.o intf_*.o

.inflate:
	$(CC) $(CFLAGS) -c -fpic tinfl.c

.modules:
	$(CC) $(CFLAGS) -c -fpic mod_*.c intf_*.c

libdarwin: library
	mv libdbticket libdbticket.dylib

libunix: library
	mv libdbticket libdbticket.so

clean:
	rm testtool libdbticket
	rm *.o
