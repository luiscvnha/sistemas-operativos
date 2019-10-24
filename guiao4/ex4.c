#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Usage: %s [flags] command\n", argv[0]);
		puts("flags:");
		puts("-i <input file>");
		puts("-o <output file>");
		return -1;
	}

	for (int i = 1; argv[i]; ++i)
		if (argv[i][0] == '-')
			switch (argv[i][1]) {
				case 'i':;
					int fd_in = open(argv[++i], O_RDONLY);
					if (fd_in < 0) {perror("open() in"); return -1;}
					dup2(fd_in, STDIN_FILENO);
					close(fd_in);
					break;
				case 'o':;
					int fd_out = open(argv[++i], O_CREAT | O_TRUNC | O_WRONLY, 0600);
					if (fd_out < 0) {perror("open() out"); return -1;}
					dup2(fd_out, STDOUT_FILENO);
					close(fd_out);
					break;
				default:
					printf("Invalid option: %s\n", argv[i]);
					return -1;
			}
		else {
			execvp(argv[i], argv + i);
			perror("execvp()");
			return -1;
		}
}
