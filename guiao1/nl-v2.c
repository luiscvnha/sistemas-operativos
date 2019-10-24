#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "readln-v2.h"


#define KiB				1024
#define BUF_T_MAX		4*KiB
#define BUF_MAX			KiB


int main(int argc, char* argv[]) {
	int fildes;

	if (argc == 1) fildes = STDIN_FILENO;
	else if (argc == 2) {
		fildes = open(argv[1], O_RDONLY);

		if (fildes < 0) {
			printf("Error opening the file.\n");
			return -2;
		}
	} else {
		printf("Usage: %s [filepath]\n", argv[0]);
		return -1;
	}

	int counter = 1, sprintf_ret;
	char buf_read[BUF_MAX], buf_counter[16];
	int bytes_read;
	struct buffer_t* buffer = create_buffer(fildes, BUF_T_MAX);

	while ((bytes_read = readln(buffer, buf_read, BUF_MAX)) > 0) {

		sprintf_ret = sprintf(buf_counter, "%6d  ", counter);
		write(STDOUT_FILENO, buf_counter, sprintf_ret);

		write(STDOUT_FILENO, buf_read, bytes_read);
		if (buf_read[bytes_read-1] == '\n')
			++counter;
		else write(STDOUT_FILENO, "\n", 1);
	}

	destroy_buffer(buffer);
	close(fildes);

	return 0;
}
