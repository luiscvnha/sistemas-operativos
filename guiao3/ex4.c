#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int r;

	//strcpy(argv[0], ":D"); /* tamanho máximo: 5 carateres "./ex3" */

	r = execvp("./ex3", argv);

	printf("execlp returned %d\n", r);
	perror("reached return");

	return 0;
}
