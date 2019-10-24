CC = gcc
CFLAGS = -Wall -Wextra
EXECUTABLES = 10mb mycat-v1 mycat-v2 nl-v1 nl-v2 mycat-v3 myhead mygrep mywc mycmp nl-v2

#all: $(EXECUTABLES)

nl-v2: nl-v2.c readln-v2.o

readln-v2.o: readln-v2.h readln-v2.c

mycmp: mycmp.c

mywc: mywc.c readln-v1.o

mygrep: mygrep.c readln-v1.o

myhead: myhead.c readln-v1.o

mycat-v3: mycat-v3.c

nl-v1: nl-v1.c readln-v1.o

readln-v1.o: readln-v1.h readln-v1.c

mycat-v2: mycat-v2.c

mycat-v1: mycat-v1.c

10mb: 10mb.c

clean:
	rm -f *.o

clean-all: clean
	rm -f $(EXECUTABLES)
