#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "person.h"

void print_usage(char* argv[]) {
	printf("Usage: %s <option> <name> <age>\n", argv[0]);
	puts("-i : Acrescentar uma pessoa");
	puts("-u : Atualizar a idade");
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		print_usage(argv);
		return -1;
	}

	Person* p;

	if (strcmp(argv[1], "-i") == 0) {

		if (strlen(argv[2]) > MAX_NAME_SIZE) {
			printf("Maximum name length is %d characters\n", MAX_NAME_SIZE);
			return -3;
		}

		p = new_person(argv[2], atoi(argv[3]));
		if (!p) return -4;

		save_person(p);

	} else if (strcmp(argv[1], "-u") == 0) {

		update_age_and_persist(argv[2], atoi(argv[3]));

	} else {

		print_usage(argv);
		return -2;

	}

	destroy_person(p);

	return 0;
}
