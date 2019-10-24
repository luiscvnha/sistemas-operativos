#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


#define BUFSIZE				1024


int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Usage: %s <file1> [file2] [file3] ...\n", argv[0]);
		return -1;
	}

	int i, fd;
	char buf[BUFSIZE];
	ssize_t bytes_read;

	for (i = 1; i < argc; ++i) {

		fd = open(argv[i], O_RDONLY);

		while ((bytes_read = read(fd, buf, BUFSIZE)) > 0) {
			write(STDOUT_FILENO, buf, bytes_read);
		}

		close(fd);
	}

	return 0;
}
