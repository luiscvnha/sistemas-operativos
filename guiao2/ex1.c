#include <unistd.h>
#include <stdio.h>

int main() {
	printf("atual: %d\npai: %d\n", getpid(), getppid());
	return 0;
}
