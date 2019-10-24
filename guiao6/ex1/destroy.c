#include <unistd.h>

int main() {
	unlink("fifo");
	return 0;
}
