#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "person.h"

Person* new_person(char* name, int age) {
	Person* r = (Person*) malloc(sizeof(Person));
	if (!r) return NULL;

	size_t n = strlen(name) + 1;
	memcpy(r->name, name, n);
	r->age = age;

	return r;
}

Person* clone_person(Person* p) {
	Person* r = (Person*) malloc(sizeof(Person));
	if (!r) return NULL;

	size_t n = strlen(p->name) + 1;
	memcpy(r->name, p->name, n);
	r->age = p->age;

	return r;
}

void destroy_person(Person* p) {
	free(p);
}

int person_age(Person* p) {
	return p->age;
}

void person_change_age(Person* p, int age) {
	p->age = age;
}

void save_person(Person* p) {
	int fd = open(FILE_NAME, O_CREAT | O_APPEND | O_WRONLY, 0600);
	if (fd < 0)
		printf("Couldn't open %s\n", FILE_NAME);
	else {
		write(fd, p, sizeof(Person));
		close(fd);
	}
}

void update_age_and_persist(char* name, int age) {
	int fd = open(FILE_NAME, O_RDWR, 0600);
	if (fd < 0)
		printf("Couldn't open %s\n", FILE_NAME);
	else {
		char found = 0, stop = 0;
		Person p;
		lseek(fd, 0, SEEK_SET);
		while (!stop)
			if (read(fd, &p, sizeof(Person)) <= 0)
				stop = 1;
			else {
				printf("Read  | Person name \"%s\" age %d\n", p.name, p.age);
				if (strcmp(p.name, name) == 0) {
					found = stop = 1;
					p.age = age;
					lseek(fd, -sizeof(Person), SEEK_CUR);
					write(fd, &p, sizeof(Person));
					printf("Wrote | Person name \"%s\" age %d\n", p.name, p.age);
				}
			}
		if (!found) puts("Not found");
		close(fd);
	}
}
