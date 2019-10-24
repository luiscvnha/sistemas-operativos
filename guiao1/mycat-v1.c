#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main() {
	char c;
	ssize_t bytes_read;

	while ((bytes_read = read(STDIN_FILENO, &c, 1)) != 0) {
		
		if (bytes_read < 0) {
			puts("Error reading characters.");
			return 1;
		} else
			write(STDOUT_FILENO, &c, 1);
	}

	return 0;
}
