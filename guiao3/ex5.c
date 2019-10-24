#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int num_progs = argc-1,
		childs[num_progs],
		i;

	for (i = 0; i < num_progs; ++i)
		if ((childs[i] = fork()) == 0) {
			execlp(argv[i+1], argv[i+1], NULL);
			perror("reached return");
			_exit(-1);
		}

	int pid, status;
	for (int j = 0; j < num_progs; ++j) {
		pid = wait(&status);
		for (i = 0; i < num_progs && childs[i] != pid; ++i);
		if (WIFEXITED(status))
			printf("%s terminou com status %d\n", argv[i+1], WEXITSTATUS(status));
		else
			printf("%s terminado\n", argv[i+1]);
	}

	return 0;
}
