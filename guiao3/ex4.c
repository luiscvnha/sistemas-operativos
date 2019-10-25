#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int r;

	r = execvp("./ex3", argv);

	printf("execlp returned %d\n", r);
	perror("reached return");

	return 0;
}
