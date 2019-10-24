#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define MiB 	1024*1024

int main(int argc, char* argv[]) {
	
	if (argc != 2) {

		printf("Usage: %s <filename>\n", argv[0]);

	} else {

		int fildes = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0600);
		if (fildes < 0) {
			puts("Error opening the file.");
			return -1;
		}

		off_t offset = lseek(fildes, 10*MiB - 1, SEEK_SET);
		if (offset < 10*MiB - 1) {
			puts("Error calling lseek().");
			return -2;
		}

		ssize_t bytes_written = write(fildes, "a", 1);
		if (bytes_written < 1) {
			puts("Error writing a byte at the end of the file.");
			return -3;
		}

		close(fildes);
	}

	return 0;
}
