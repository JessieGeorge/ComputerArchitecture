CC = gcc
FLAGS = -Wall
COMPILE = $(CC) $(FLAGS)

all: calc format

calc: calc.c calc.h
	$(COMPILE) -g -o calc calc.c
format: format.c format.h
	$(COMPILE) -g -o format format.c
clean: 
	rm -rf *.o calc format

