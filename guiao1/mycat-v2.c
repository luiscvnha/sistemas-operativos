#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: %s <N>\n", argv[0]);
		return -1;
	}

	int N = atoi(argv[1]);
	if (N <= 0) {
		puts("Insert a positive number");
		return -2;
	}

	char buf[N];	
	ssize_t bytes_read;

	while (1) {

		bytes_read = read(STDIN_FILENO, buf, N);

		if (bytes_read < 0) {
			puts("Error reading characters.");
			return -2;
		} else if (bytes_read == 0)
			return 0;
		else
			write(STDOUT_FILENO, buf, bytes_read);
	}

	return 0;
}
