#include <unistd.h>
#include <fcntl.h>

#define IN_PATH		"/etc/passwd"
#define OUT_PATH	"out.log"
#define ERR_PATH	"err.log"

#define KiB			1024

int main() {
	// redirecionamentos
	int fd_in, fd_out, fd_err;

	fd_in = open(IN_PATH, O_RDONLY);
	if (fd_in < 0) return -1;
	fd_out = open(OUT_PATH, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (fd_out < 0) return -1;
	fd_err = open(ERR_PATH, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (fd_err < 0) return -1;

	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	dup2(fd_err, STDERR_FILENO);

	close(fd_in);
	close(fd_out);
	close(fd_err);
	// ###

	char buffer[KiB];
    int n;
    while ((n = read(STDIN_FILENO, buffer, KiB)) > 0) {
        write(STDOUT_FILENO, buffer, n);
        write(STDERR_FILENO, buffer, n);
	}

	return 0;
}
