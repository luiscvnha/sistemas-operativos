#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "server.h"

int main() {
	puts("Starting server...");
	puts("Creating pipe...");
	mkfifo(PIPE, PERM);

	puts("Opening pipe...");
	int in = open(PIPE, O_RDONLY);
	if (in < 0) {perror("open() in"); return -1;}
	
	puts("Opening log file...");
	int log = open(LOG, O_CREAT | O_WRONLY | O_APPEND, PERM);
	if (log < 0) {perror("open() log"); return -1;}
	write(log, SEP, strlen(SEP));

	puts("Reading...");
	char buf[BUFSIZE];
	int n;
	while (1) {
		n = read(in, buf, BUFSIZE);
		if (n <= 0);
		else if (strncmp(buf, "stop", 4) == 0)
			break;
		else {
			write(log, buf, n);
			write(log, "\n", 1);
		}
	}

	puts("Closing server...");
	close(in);
	close(log);
	unlink(PIPE);
	return 0;
}
