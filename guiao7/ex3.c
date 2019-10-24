#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "guiao7.h"

#define NUM_LINES				5
#define NUM_COLS				(INT_MAX / 1000)
#define NUM_CELLS				(NUM_LINES * NUM_COLS)

int main(int argc, char* argv[]) {
	srand(time(NULL));
	setbuf(stdout, NULL);

	int target;
	int range = NUM_COLS / 4;

	if (argc < 2)
		target = rand() % range;
	else
		target = atoi(argv[1]);

	if (target > range) {
		printf("Number is too big (max is %d)\n", range);
		return 0;
	}

	// Matrix
	printf("Initializing matrix [-] ");
	char bars[] = "|/-\\";
	int* matrix[NUM_LINES];

	for (int i = 0; i < NUM_LINES; ++i)
		matrix[i] = malloc(NUM_COLS * sizeof(int));
	int progress = 0;

	for (int i = 0; i < NUM_LINES; ++i)
		for (int j = 0; j < NUM_COLS; ++j) {
			if (!(progress % 4000)) {
				printf("\b\b\b%c] ", bars[(progress / 4000) % 4]);
				//fflush(stdout);
			}
			matrix[i][j] = rand() % range;
			//usleep(10);
			++progress;
		}

	printf("\nSearching for %d in matrix\n", target);

	// Forks
	int childs[NUM_LINES];
	for (int i = 0; i < NUM_LINES; ++i)
		if (!(childs[i] = fork())) {
			for (int j = 0; j < NUM_COLS; ++j) {
				if (matrix[i][j] == target)
					_exit(1);
				//usleep(1000);
			}
			_exit(0);
		}

	// Waiting
	int pid, status;
	while ((pid = wait(&status)) != -1) {
		int i;
		for (i = 0; i < NUM_LINES && pid != childs[i]; ++i);
		childs[i] = 0;
		if (WEXITSTATUS(status)) {
			printf("Child %d found the target on line %d\n", pid, i);
			for (int j = 0; j < NUM_LINES; ++j)
				if (childs[j])
					kill(childs[j], SIGINT);
		} else
			printf("Child %d didn't find nothing on line %d\n", pid, i);
	}

	return 0;
}
