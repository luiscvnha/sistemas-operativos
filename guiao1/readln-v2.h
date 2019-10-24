#ifndef readln_v2_h
#define readln_v2_h


struct buffer_t {
	int fd;
	char* buf;
	size_t capacity;
	size_t size;
	size_t start;
};




struct buffer_t* create_buffer(int fildes, size_t size);


int destroy_buffer(struct buffer_t* buffer);


ssize_t readln(struct buffer_t* buffer, void* buf, size_t nbyte);


#endif
