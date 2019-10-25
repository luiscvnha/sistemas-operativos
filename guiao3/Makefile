CC = gcc
CFLAGS = -Wall -Wextra
EXE = ex1 ex2 ex3 ex4 ex5 ex6


all: $(EXE)

ex6: ex6.c mysystem.o

mysystem.o: mysystem.h mysystem.c

ex5: ex5.c

ex4: ex4.c

ex3: ex3.c

ex2: ex2.c

ex1: ex1.c

clean:
	rm -f *.o $(EXE)
