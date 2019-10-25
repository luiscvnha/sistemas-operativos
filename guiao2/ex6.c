#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_LINES		5
#define NUM_COLS		10000
#define NUM_CELLS		(NUM_LINES * NUM_COLS)

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int target;
	int range = NUM_COLS * 4;
	if (argc < 2)
		target = rand() % range;
	else
		target = atoi(argv[1]);

	if (target > range) {
		printf("Number is too big (max is %d)\n", range);
		return 0;
	}

	// Matrix
	puts("Initializing matrix");
	int matrix[NUM_LINES][NUM_COLS];
	for (int i = 0; i < NUM_LINES; ++i)
		for (int j = 0; j < NUM_COLS; ++j)
			matrix[i][j] = rand() % range;
	printf("Searching for %d in matrix\n", target);

	// Forks
	int childs[NUM_LINES];
	for (int i = 0; i < NUM_LINES; ++i)
		if (!(childs[i] = fork())) {
			for (int j = 0; j < NUM_COLS; ++j)
				if (matrix[i][j] == target) _exit(1);
			_exit(0);
		}

	// Waiting
	int pid, status;
	while ((pid = wait(&status)) != -1) {
		int i;
		for (i = 0; i < NUM_LINES && pid != childs[i]; ++i);
		if (WEXITSTATUS(status))
			printf("Child %d found the target on line %d\n", pid, i);
		else
			printf("Child %d didn't find nothing on line %d\n", pid, i);
	}

	return 0;
}
