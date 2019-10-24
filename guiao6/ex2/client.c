#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "server.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: %s \"message\"\n", argv[0]);
		return 0;
	}

	int out = open(PIPE, O_WRONLY);
	if (out < 0) {
		puts("Server's offline");
		return -1;
	}

	write(out, argv[1], strlen(argv[1]));

	close(out);
	return 0;
}
