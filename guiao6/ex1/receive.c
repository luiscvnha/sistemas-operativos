#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
	int fd_in = open("fifo", O_RDONLY);
	if (fd_in < 0) {perror("open()"); return -1;}

	char buf[10];
	int n;
	while ((n = read(fd_in, buf, 10)) > 0)
		write(STDOUT_FILENO, buf, n);

	close(fd_in);

	return 0;
}
