#include <stdio.h>
#include <string.h>
#include "mysystem.h"


int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Usage: %s <command>\n", argv[0]);
		return -1;
	}

	// preparar o input de mySystem()
	int i,
		num_args = argc-1,
		len = num_args, // número de espaços + '\0'
		offset;

	for (i = 1; i <= num_args; ++i)
		len += strlen(argv[i]);

	char command[len];

	offset = sprintf(command, "%s", argv[1]);
	for (i = 2; i <= num_args; ++i)
		offset += sprintf(command+offset, " %s", argv[i]);
	// ###

	int mySystem_ret = mySystem(command);
	if (mySystem_ret == -1)
		printf("Erro a executar mySystem()\n");
	else
		printf("mySystem() returned %d\n", mySystem_ret);

	return 0;
}
