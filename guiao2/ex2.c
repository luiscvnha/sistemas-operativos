#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
	pid_t pid;
	int status;

	if ((pid = fork()) == 0) {
		printf("---processo filho---\npai: %d\tatual: %d\n", getppid(), getpid());
		_exit(0);
	} else {
		printf("---processo pai---\npai: %d\tatual: %d\tfilho: %d\n", getppid(), getpid(), pid);
		wait(&status);
	}

	return 0;
}
