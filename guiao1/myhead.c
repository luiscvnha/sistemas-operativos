#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "readln-v1.h"


#define MAX							1024 // max chars to read per line
#define DEFAULT_N_LINES				10


int main(int argc, char* argv[]) {
	if (argc < 2) {
		char c;
		int n;
		while ((n = read(STDIN_FILENO, &c, 1)) > 0)
			write(STDOUT_FILENO, &c, n);
	} else {
		int n_lines;
		int file;

		if (argv[1][0] == '-' && argv[1][1] != '\0') {
			n_lines = atoi(argv[1] + 1);
			file = 2;
		} else {
			n_lines = DEFAULT_N_LINES;
			file = 1;
		}

		char n_files = argc - file;
		int line, bytes_read;
		char buf[MAX];

		if (n_files == 0)
			for (line = 0; line < n_lines && (bytes_read = readln(STDIN_FILENO, buf, MAX)) > 0; ++line)
				write(STDOUT_FILENO, buf, bytes_read);
		else {
			char print_newln = 0;

			for (; file < argc; ++file) {
				int fd;
				fd = open(argv[file], O_RDONLY);
				if (fd < 0) continue;

				if (print_newln) write(STDOUT_FILENO, "\n", 1);

				if (n_files > 1) {
					char header[strlen(argv[file]) + 10];
					sprintf(header, "==> %s <==\n", argv[file]);
					write(STDOUT_FILENO, header, strlen(header));
				}

				for (line = 0; line < n_lines && (bytes_read = readln(fd, buf, MAX)) > 0; ++line)
					write(STDOUT_FILENO, buf, bytes_read);

				close(fd);
				print_newln = 1;
			}
		}
	}

	return 0;
}
