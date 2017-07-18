CC = gcc
FLAGS = -Wall
COMPILE = $(CC) $(FLAGS)

all: y86emul

y86emul: y86emul.c y86emul.h
	$(COMPILE) -g -o y86emul y86emul.c

clean:
	rm -rf *.o y86emul