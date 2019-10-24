#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "readln-v2.h"


#define charbuf					((char*) buf)


static ssize_t refill(struct buffer_t* buffer) {
	ssize_t bytes_read = read(buffer->fd, buffer->buf, buffer->capacity);

	buffer->start = 0;
	buffer->size = bytes_read;

	return bytes_read;
}


static size_t charncpy(char* buf, struct buffer_t* buffer, size_t n) {
	size_t i = buffer->start,
		   j,
		   max = buffer->size < n ? buffer->size : n;
	char* buff = buffer->buf;

	for (j = 0; buff[i] != '\n' && j < max; ++i, ++j)
		buf[j] = buff[i];
	if (buff[i] == '\n' && j < max) buf[j++] = buff[i++];

	buffer->start = i;
	buffer->size -= j;

	return j;
}


struct buffer_t* create_buffer(int fildes, size_t size) {
	struct buffer_t* r = (struct buffer_t*) malloc(sizeof(struct buffer_t));

	if (!r) return NULL;

	r->buf = (char*) malloc(size);

	if (!r->buf) {free(r); return NULL;}

	r->fd = fildes;
	r->capacity = size;
	r->size = 0;
	r->start = 0;

	return r;
}


int destroy_buffer(struct buffer_t* buffer) {
	int fd = buffer->fd;

	free(buffer->buf);
	free(buffer);

	return fd;
}


ssize_t readln(struct buffer_t* buffer, void* buf, size_t nbyte) {
	size_t r = 0;
	ssize_t bytes_read;

	while (r < nbyte && (r == 0 || charbuf[r-1] != '\n')) {
		if (buffer->size == 0) {
			bytes_read = refill(buffer);
			if (bytes_read == 0) return (ssize_t) r;
			else if (bytes_read < 0) return bytes_read;
		}
		r += charncpy(charbuf + r, buffer, nbyte - r);
	}

	return (ssize_t) r;
}
