#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "readln-v1.h"


#define INIT_BUFSIZE			32


static char* strnstr(const char* haystack, const char* needle, size_t n) {
	unsigned int i, j, auxi;
	size_t len = strlen(needle);

	for (i = 0; i < n && haystack[i] != '\0'; ++i) {
		for (auxi = i, j = 0;
			auxi < n && haystack[auxi] != '\0' && haystack[auxi] == needle[j];
			++auxi, ++j);
		if (j == len) return (char*) haystack + i;
	}

	return NULL;
}


int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Usage: %s <filter> <file1> [file2] [file3] ...\n", argv[0]);
		return -1;
	}

	int file = 2, 				// indice para os ficheiros em argv[]
	    n_files = argc - file,	// número de ficheiros para procurar
	    fd,						// descritor de ficheiro
	    bufsize = INIT_BUFSIZE, // tamanho do buffer para a leitura das linhas
	    aux,					// bytes lidos pela função readln
	    bytes_read, 			// bytes lidos pela função readln por linha
	    N, 						// número máximo de bytes para a readln ler
	    offset; 				// indice do buf para onde a readln escreve
	char* filter = argv[1];
	char* buf = (char*) malloc(bufsize);

	for (; file < argc; ++file) {
		fd = open(argv[file], O_RDONLY);
		if (fd < 0) continue;

		N = bufsize;
		bytes_read = offset = 0;
		do {
			aux = readln(fd, buf + offset, N);
			if (aux < 0) perror("readln");
			else {
				bytes_read += aux;
				if (bytes_read == bufsize && buf[bytes_read-1] != '\n') {
					offset = bytes_read;
					N = bufsize;
					bufsize *= 2;
					buf = (char*) realloc(buf, bufsize);
				} else {
					if (strnstr(buf, filter, bytes_read)) {
						if (n_files > 1) {
							write(STDOUT_FILENO, argv[file], strlen(argv[file]));
							write(STDOUT_FILENO, ":", 1);
						}
						write(STDOUT_FILENO, buf, bytes_read);
						if (buf[bytes_read-1] != '\n') write(STDOUT_FILENO, "\n", 1);
					}
					N = bufsize;
					offset = bytes_read = 0;
				}
			}
		} while (aux > 0);

		close(fd);
	}

	free(buf);
	return 0;
}
