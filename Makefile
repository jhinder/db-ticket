CC=gcc

all: testtool

testtool:
	gcc -o testtool testtool.c mod_*.c

clean:
	rm testtool