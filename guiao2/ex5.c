#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
	int i;

	for (i = 1; i <= 10; ++i)
		if (fork()) break;
		else printf("processo #%02d criado | pai=%d | pid=%d\n", i, getppid(), getpid());

	int pid, status;
	while ((pid = wait(&status)) != -1)
		printf("pid=%d | %d terminado\n", getpid(), pid);

	return 0;
}
