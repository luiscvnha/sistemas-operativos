#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
	int pid;

	setbuf(stdout, NULL);

	printf("PROCESSO\tPAI\tATUAL\tEXIT STATUS\n");
	for (int i = 1; i <= 10; ++i)
		if ((pid = fork()) == 0) {
			printf("%d\t\t%d\t%d", i, getppid(), getpid());
			_exit(i);
		} else {
			int status;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				printf("\tprocesso %d terminado com código de saída %d\n", pid, WEXITSTATUS(status));
			else
				printf("\tprocesso %d terminado\n", pid);
		}

	return 0;
}
