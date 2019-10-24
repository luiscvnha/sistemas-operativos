#include <sys/stat.h>

int main() {
	mkfifo("fifo", 0600);
	return 0;
}
