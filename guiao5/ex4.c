#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define IN 			0
#define OUT			1

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

		execlp("wc", "wc", "-l", NULL);
		perror("execlp()@filho");

		close(STDIN_FILENO);

		_exit(1);
	}
	// PAI
	close(pd[IN]);
	dup2(pd[OUT], STDOUT_FILENO);
	close(pd[OUT]);

	execlp("ls", "ls", "/etc", NULL);
	perror("execlp()@pai");

	close(STDOUT_FILENO);

	wait(NULL);

	return 0;
}
