#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
	int fd_out = open("fifo", O_WRONLY);
	if (fd_out < 0) {perror("open()"); return -1;}

	char buf[10];
	int n;
	while ((n = read(STDIN_FILENO, buf, 10)) > 0)
		write(fd_out, buf, n);

	close(fd_out);

	return 0;
}
