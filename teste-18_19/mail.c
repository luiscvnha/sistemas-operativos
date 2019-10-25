#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>


#define STDIN			STDIN_FILENO
#define STDOUT			STDOUT_FILENO

#define MAX_PROC		10
#define LINE_SIZE		10


static int num_proc = 0;


static void sig_child_handler(int signum) {
	(void) signum;
	int status;
	while (waitpid(-1, &status, WNOHANG) > 0) {
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0);
		else puts("Erro a executar 'mail'");
		--num_proc;
	}
}


int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: %s <filepath>\n", argv[0]);
		return 1;
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("Erro a abrir ficheiro");
		return 2;
	}

	signal(SIGCHLD, sig_child_handler);

	int pip[2], pid;
	char buf[LINE_SIZE];

	while (read(fd, buf, LINE_SIZE) > 0) {
		while (num_proc >= MAX_PROC) pause();

		pipe(pip);

		if ((pid = fork()) == 0) {
			close(pip[STDOUT]);
			dup2(pip[STDIN], STDIN);
			close(pip[STDIN]);
			char* email = "a00000@alunos.uminho.pt";
			for (int i = 0; i < 6; ++i) email[i] = buf[i];
			execlp("mail", "mail", "-s", "Sistemas_Operativos", email, NULL);
			close(STDIN);
			_exit(1);
		} else if (pid > 0) {
			close(pip[STDIN]);
			write(pip[STDOUT], buf, LINE_SIZE);
			close(pip[STDOUT]);
		} else {
			perror("Erro a dar fork");
			while (wait(NULL) > 0);
			return 3;
		}

		++num_proc;
	}

	while (num_proc > 0) pause();

	return 0;
}
