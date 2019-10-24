CC = gcc
CFLAGS = -Wall -Wextra
EXE = client server


all: $(EXE)

client: server.h client.c

server: server.h server.c

clean:
	rm -f pipe log.txt $(EXE)
