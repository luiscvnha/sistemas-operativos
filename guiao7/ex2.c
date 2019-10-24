#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "guiao7.h"

#define NEXT(current)				(((current) + 1) % num_processes)

int* pids;
int num_processes;

void switchProccess(int pidStop, int pidStart) {
	if (pidStop != -1)
		kill(pidStop, SIGSTOP);
	kill(pidStart, SIGCONT);
}

void child_stopped() {
	int i,
		pid;

	while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
		for (i = 0; pid != pids[i] && i < num_processes; ++i);
		--num_processes;
		while (i < num_processes) {
			pids[i] = pids[i + 1];
			++i;
		}
	}
	signal(SIGCHLD, child_stopped);
}

int main(int argc, char* argv[]) {
	if (argc < 2)
		return -1;

	num_processes = argc - 1;
	pids = (int*) malloc(num_processes * sizeof(int));

	for (int i = 0; i < argc - 1; ++i) {
		printf("Launching %s\n", argv[i+1]);
		if ((pids[i] = fork()) == 0) {
			execlp(argv[i+1], argv[i+1], NULL);
			_exit(1);
		}
		kill(pids[i], SIGSTOP);
	}

	int current = 0,
		first = 1;
	signal(SIGCHLD, child_stopped);

	while (num_processes > 0) {
		sleep(1);
		if (num_processes < 1)
			break;
		printf("Number of childs: %d\n", num_processes);
		printf("Switching %d <-> %d\n", current, NEXT(current));
		printf("%d <-> %d\n", pids[current], pids[NEXT(current)]);
		switchProccess(first ? -1 : pids[current], pids[NEXT(current)]);
		current = NEXT(current);
		first = 0;
	}

	free(pids);
	return 0;
}
