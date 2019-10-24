#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
	int i;

	for (i = 1; i <= 10; ++i)
		if (fork() == 0) _exit(i);

	int pid, status;
	for (i = 1; i <= 10; ++i) {
		pid = wait(&status);
		if (WIFEXITED(status))
			printf("processo %d terminou com status %d\n", pid, WEXITSTATUS(status));
		else
			printf("processo %d terminou\n", pid);
	}

	return 0;
}
