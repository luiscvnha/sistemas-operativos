#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>


#define STDIN			STDIN_FILENO
#define STDOUT			STDOUT_FILENO

#define OUT_FILE_PERM	0600
#define MAX_PROC		10
#define LINE_SIZE		101		// ... + '\n'


static int pids[MAX_PROC],
		   num_proc = 0;
static unsigned char time[MAX_PROC];


static void sig_child_handler(int signum) {
	(void) signum;

	int status, pid;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		int i;
		for (i = 0; pids[i] != pid; ++i);

		if (WIFEXITED(status) && WEXITSTATUS(status) == 0);
		else puts("Erro a executar 'agrep'");

		for (; i < num_proc - 1; ++i) {
			pids[i] = pids[i+1];
			time[i] = time[i+1];
		}

		--num_proc;
	}
}


static void sig_alarm_handler(int signum) {
	(void) signum;
	alarm(1);
	for (int i = 0; i < num_proc; ++i)
		if (time[i] >= 60) {
			kill(pids[i], SIGTERM);
			for (int j = i; j < num_proc - 1; ++j) {
				pids[j] = pids[j+1];
				time[j] = time[j+1];
			}
			--num_proc;
		} else ++time[i];
}


int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Usage: %s <word> <filepath>\n", argv[0]);
		return 1;
	}

	int fd_out = open(argv[2], O_CREAT | O_WRONLY | O_APPEND, OUT_FILE_PERM);
	if (fd_out < 0) {
		perror("Erro a abrir ficheiro");
		return 2;
	}

	signal(SIGCHLD, sig_child_handler);
	signal(SIGALRM, sig_alarm_handler);

	char buf[LINE_SIZE];
	int bytes_read, pid;

	alarm(1);

	while ((bytes_read = readline(STDIN, buf, LINE_SIZE)) > 0) {
		while (num_proc >= MAX_PROC) pause();

		buf[bytes_read-1] = '\0';		// '\n' -> '\0'

		if ((pid = fork()) == 0) {
			dup2(fd_out, STDOUT);
			close(fd_out);

			execlp("agrep", "agrep", argv[1], buf, NULL);

			close(STDOUT);
			_exit(1);
		} else if (pid > 0) {
			pids[num_proc] = pid;
			time[num_proc] = 0;
		} else {
			perror("Erro a dar fork");
			while (wait(NULL) > 0);
			return 3;
		}

		++num_proc;
	}

	while (num_proc > 0) pause();

	close(fd_out);

	return 0;
}
