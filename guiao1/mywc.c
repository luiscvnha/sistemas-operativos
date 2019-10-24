#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include "readln-v1.h"


#define BUFSIZE			1024 // max chars to read per line

#define NEW_LINE		0
#define WORD			1
#define BYTE			2


static void counter(const char* str, int size, int r[3]) {
	int i;
	char inWord = 0;

	for (i = 0; i < size; ++i) {
		if (str[i] == '\n') ++r[NEW_LINE];
		if (!isspace(str[i])) inWord = 1;
		else if (inWord) {++r[WORD]; inWord = 0;}
	}
	if (inWord) ++r[WORD];

	r[BYTE] += i;
}


int main(int argc, char* argv[]) {
	char buf[BUFSIZE];
	int bytes_read;

	if (argc < 2) {

		int r[3] = {0, 0, 0};

		while ((bytes_read = readln(STDIN_FILENO, buf, BUFSIZE)) > 0)
			counter(buf, bytes_read, r);

		if (bytes_read == 0) printf("%7d %7d %7d\n", r[0], r[1], r[2]);
		else perror("readln");

	} else {

		int i,
			fd,
			file = 1,
			n_files = argc - file,
			r[n_files == 1 ? 1 : n_files+1][3];

		for (; file < argc; ++file) {
			i = file - 1;
			fd = open(argv[file], O_RDONLY);

			r[i][0] = r[i][1] = r[i][2] = 0;
			while ((bytes_read = readln(fd, buf, BUFSIZE)) > 0)
				counter(buf, bytes_read, r[i]);

			close(fd);

			if (bytes_read == 0) printf("%7d %7d %7d %s\n", r[i][0], r[i][1], r[i][2], argv[file]);
			else perror("readln");
		}

		if (n_files > 1) {
			r[file][0] = r[file][1] = r[file][2] = 0;
			for (i = 0; i < n_files; ++i) {
				r[file][0] += r[i][0];
				r[file][1] += r[i][1];
				r[file][2] += r[i][2];
			}
			printf("%7d %7d %7d total\n", r[file][0], r[file][1], r[file][2]);
		}
	}

	return 0;
}
