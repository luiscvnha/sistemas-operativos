#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include "mysystem.h"

#define WEXITSSTATUS(status)		((signed char) (WEXITSTATUS(status)))

int mySystem(const char* command) {
	if (command == NULL || command[0] == '\0') return -1;

	char* _command = strdup(command);
	if (!command) return -1;

	// parse do input
	int i, argc = 1;

	for (i = 0; _command[i] != '\0'; ++i)
		if (_command[i] == ' ') ++argc;

	char* argv[argc+1];
	argv[argc] = NULL;

	argv[0] = strtok(_command, " ");
	for (i = 1; i < argc && (argv[i] = strtok(NULL, " ")); ++i);
	// ###

	if (fork() == 0) {
		execvp(argv[0], argv);
		// se correr bem:
		//_exit(0);
		// se der erro:
		perror("execvp()");
		_exit(-1);
	}

	int status;
	wait(&status);

	if (WIFEXITED(status))
		return WEXITSSTATUS(status);
	else
		return -1;
}
