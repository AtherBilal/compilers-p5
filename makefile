#makefile for P2

all: *.c
	gcc -std=c99 -g -Wall -o staticSem  *.c

clean:
	$(RM) staticSem
