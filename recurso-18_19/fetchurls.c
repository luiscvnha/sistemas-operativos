#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


#define PID				0
#define IN				1
#define STDIN			STDIN_FILENO
#define STDOUT			STDOUT_FILENO

#define MAX_PROC		8
#define LINE_SIZE		101				// url + '\n'

#define ABORT			kill(getpid(), SIGQUIT)


static int procs[MAX_PROC][2],			// PID, IN
		   num_proc = 0,				// número de processos a correr
		   url_list_cap,
		   url_list_size = 0;
static char* url_list;


static void quit_handler(int signum) {
	(void) signum;
	while (wait(NULL) > 0);
	free(url_list);
}


static void add_urls_to_list(int proc_num) {
	int bytes_read;

	while ((bytes_read = readline(procs[proc_num][IN], url_list + url_list_size, LINE_SIZE)) > 0) {
		url_list_size += bytes_read;

		// REALLOCATING =============================================
		if (url_list_size > url_list_cap - LINE_SIZE) {
			char* aux = (char*) realloc(url_list, url_list_cap << 1);
			if (!aux) {
				perror("Erro a realocar lista de urls");
				ABORT;
			}
			url_list = aux;
			url_list_cap <<= 1;
		}
		// ==========================================================
	}
}


static void sig_child_handler(int signum) {
	(void) signum;

	int status, pid;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		int i;
		for (i = 0; procs[i][PID] != pid; ++i);

		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) add_urls_to_list(i);
		else puts("Erro a executar 'fetchurls'");

		close(procs[i][IN]);

		for (; i < num_proc-1 ; ++i) {
			procs[i][PID] = procs[i+1][PID];
			procs[i][IN] = procs[i+1][IN];
		}

		--num_proc;
	}
}


int main() {
	url_list_cap = 100 * LINE_SIZE;
	url_list = (char*) malloc(url_list_cap);
	if (!url_list) {
		perror("Erro a dar a alocar lista de urls");
		return 1;
	}

	signal(SIGINT, quit_handler);
	signal(SIGQUIT, quit_handler);
	signal(SIGCHLD, sig_child_handler);

	int pid, pip[2];

	// FETCHURLS ===========================================
	int bytes_read;
	char url[LINE_SIZE];

	while ((bytes_read = readline(STDIN, url, LINE_SIZE)) > 0) {
		while (num_proc >= MAX_PROC) pause();

		pipe(pip);

		if ((pid = fork()) == 0) {
			dup2(pip[STDIN], STDIN);
			dup2(pip[STDOUT], STDOUT);
			close(pip[STDIN]);
			close(pip[STDOUT]);

			execlp("fetchurls", "fetchurls", NULL);

			close(STDIN);
			close(STDOUT);
			_exit(1);
		} else if (pid > 0) {
			write(pip[STDOUT], url, bytes_read);
			close(pip[STDOUT]);

			procs[num_proc][PID] = pid;
			procs[num_proc][IN] = pip[STDIN];
		} else {
			perror("Erro a dar fork");
			ABORT;
		}

		++num_proc;
	}

	while (num_proc > 0) pause();

	// UNIQ ================================================
	signal(SIGCHLD, SIG_DFL);		// default handling

	pipe(pip);

	if ((pid = fork()) == 0) {
		close(pip[STDOUT]);
		dup2(pip[STDIN], STDIN);
		close(pip[STDIN]);

		execlp("uniq", "uniq", NULL);

		close(STDIN);
		_exit(1);
	} else if (pid > 0) {
		close(pip[STDIN]);
		write(pip[STDOUT], url_list, url_list_size);
		close(pip[STDOUT]);

		int status;
		wait(&status);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0);
		else puts("Erro a executar 'uniq'");
	} else {
		perror("Erro a dar fork");
		free(url_list);
		return 2;
	}

	// EXITING =============================================
	free(url_list);

	return 0;
}
