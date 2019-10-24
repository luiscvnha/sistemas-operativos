CC = gcc
CFLAGS = -Wall

pessoas: pessoas.o person.o

person.o: person.h person.c

pessoas.o: person.h pessoas.c

clean:
	rm -f pessoas *.o pessoas.txt
