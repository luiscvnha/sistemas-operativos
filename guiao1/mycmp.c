#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Usage: %s <file1> <file2>\n", argv[0]);
		return -1;
	}

	int fd1, fd2, line, byte;
	char c1, c2;

	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_RDONLY);

	for (byte = line = 1; read(fd1, &c1, 1) > 0 && read(fd2, &c2, 1) > 0; ++byte)
		if (c1 != c2) break;
		else if (c1 == '\n') ++line;

	close(fd1);
	close(fd2);

	if (c1 == c2) puts("Files are the same.");
	else printf("%s %s differ: byte %d, line %d\n", argv[1], argv[2], byte, line);

	return 0;
}
