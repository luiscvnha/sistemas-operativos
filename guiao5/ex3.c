#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define IN 			0
#define OUT			1

#define KiB			1024

// Mensagem Pai -> Filho
int main() {
	int pd[2];

	if (pipe(pd) == -1) {
		perror("pipe()");
		return -1;
	}

	if (fork() == 0) {
		// FILHO
		close(pd[OUT]);

		dup2(pd[IN], STDIN_FILENO);

		close(pd[IN]);

		execlp("wc", "wc", NULL);

		close(STDIN_FILENO);

		_exit(1);
	}
	// PAI
	close(pd[IN]);

	int n; char buf[KiB];
	while ((n = read(STDIN_FILENO, buf, KiB)) > 0)
		write(pd[OUT], buf, n);

	close(pd[OUT]);

	wait(NULL);

	return 0;
}
