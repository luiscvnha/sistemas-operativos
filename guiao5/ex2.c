#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>


#define IN 			0
#define OUT			1

#define	MSG			"QWERTYUIOPASDFGHJKLZXCVBNM"


// Mensagem Pai -> Filho
int main() {
	int len = strlen(MSG);
	char buf[len];
	int pd[2];

	if (pipe(pd) == -1) {
		perror("pipe()");
		return -1;
	}

	if (fork() == 0) {
		// FILHO
		close(pd[OUT]);

		int n;
		while ((n = read(pd[IN], buf, len)) > 0) {
			write(STDOUT_FILENO, buf, n);
			write(STDOUT_FILENO, "\n", 1);
		}

		close(pd[IN]);

		_exit(1);
	}
	// PAI
	int status;

	close(pd[IN]);

	write(pd[OUT], MSG, len);

	close(pd[OUT]);

	wait(&status);

	return 0;
}
