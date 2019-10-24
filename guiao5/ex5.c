#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#define IN 			0
#define OUT			1

int main() {
	char** comandos[4];
	char* grep[] = {"grep", "-v", "^#", "/etc/passwd", NULL};
	char* cut[] = {"cut", "-f7", "-d:", NULL};
	char* uniq[] = {"uniq", NULL};
	char* wc[] = {"wc", "-l", NULL};
	comandos[0] = grep;
	comandos[1] = cut;
	comandos[2] = uniq;
	comandos[3] = wc;

	int pipeAnt = STDIN_FILENO;
	int proxPipe[2];
	int n = 4;

	for (int i = 0; i < n; ++i) {
		if (i < n - 1)
			pipe(proxPipe);
		if (fork() == 0) {
			if (i < n - 1) {
				close(proxPipe[IN]);
				dup2(proxPipe[OUT], STDOUT_FILENO);
				close(proxPipe[OUT]);
			}
			if (i > 0) {
				dup2(pipeAnt, STDIN_FILENO);
				close(pipeAnt);
			}
			execvp(comandos[i][0], comandos[i]);
			_exit(1);
		}
		if (i < n - 1)
			close(proxPipe[OUT]);
		if (i > 0)
			close(pipeAnt);
		pipeAnt = proxPipe[IN];
		wait(NULL);
	}

	return 0;
}
