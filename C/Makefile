CC=gcc
CFLAGS=-O3

all: testtool

testtool:
	$(CC) -o testtool testtool.c libmain.c intf_file_src.c mod_*.c

dynamic:
	$(CC) $(CFLAGS) -c -fpic libmain.c intf_file_src.c mod_*.c
	$(CC) -shared -o libdbticket.so libmain.o intf_file_src.o mod_*.o
	rm *.o

clean-testtool:
	rm testtool

clean-dynamic:
	rm libdbticket.so

clean: clean-testtool clean-dynamic
