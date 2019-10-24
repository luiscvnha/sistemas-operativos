#include <unistd.h>
#include <fcntl.h>


#define charbuf				((char*) buf)


ssize_t readln(int fildes, void* buf, size_t nbyte) {
	size_t i;

	for (i = 0; i < nbyte && read(fildes, charbuf + i, 1) > 0; ++i) {
		if (charbuf[i] == '\0') return i;
		if (charbuf[i] == '\n') return i+1;
	}

	return i;
}
