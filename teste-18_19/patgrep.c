#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>


#define PID				0		// procs[...][0] -> PID
#define IN				1		// procs[...][1] -> STDIN

#define STDIN			STDIN_FILENO
#define STDOUT			STDOUT_FILENO

#define CHUNK_SIZE		128
#define MAX_PROC		8


static int procs[MAX_PROC][2],
		   num_proc = 0,			// número de processos a correr
		   counter = 0;


static void sig_child_handler(int signum) {
	(void) signum;

	int status, pid;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		int i;
		for (i = 0; procs[i][PID] != pid; ++i);

		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			char c;
			if (read(procs[i][IN], &c, 1) > 0) ++counter;
		} else puts("Erro a executar 'patgrep'");

		close(procs[i][IN]);

		for (; i < num_proc - 1; ++i) {
			procs[i][PID] = procs[i+1][PID];
			procs[i][IN] = procs[i+1][IN];
		}
		
		--num_proc;
	}
}


int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: %s <pattern>\n", argv[0]);
		return 1;
	}

	signal(SIGCHLD, sig_child_handler);

	int pid, pip[2];
	char buf[CHUNK_SIZE];

	while (read(STDIN, buf, CHUNK_SIZE) > 0) {
		while (num_proc >= MAX_PROC) pause();

		pipe(pip);

		if ((pid = fork()) == 0) {
			dup2(pip[STDIN], STDIN);
			dup2(pip[STDOUT], STDOUT);
			close(pip[STDIN]);
			close(pip[STDOUT]);
			execlp("patgrep", "patgrep", argv[1], NULL);
			close(STDIN);
			close(STDOUT);
			_exit(1);
		} else if (pid > 0) {
			write(pip[STDOUT], buf, CHUNK_SIZE);
			close(pip[STDOUT]);
			procs[num_proc][PID] = pid;
			procs[num_proc][IN] = pip[STDIN];
		} else {
			perror("Erro a dar fork");
			while (wait(NULL) > 0);
			return 2;
		}

		++num_proc;
	}

	while (num_proc > 0) pause();

	printf("%d\n", counter);

	return 0;
}
