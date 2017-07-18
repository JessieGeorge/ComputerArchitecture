CC = gcc
FLAGS = -m32
COMPILE = $(CC) $(FLAGS)

formula: formula.c formula.h nCr.s nCr.h
	$(COMPILE) -o formula formula.c nCr.s 
	
clean: 
	rm -rf *.o formula