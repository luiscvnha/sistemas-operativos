#include <unistd.h>
#include <stdio.h>

int main() {
	int r;
    
    r = execlp("ls", "ls", "-l", NULL);

    printf("execlp returned %d\n", r);
    perror("reached return");

    return 0;
}
