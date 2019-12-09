#makefile for P3

all: *.c
	gcc -std=c99 -g -Wall -o compile  *.c

clean:
	$(RM) compile
	$(RM) *.asm
