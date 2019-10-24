#include <stdio.h>
#include <unistd.h>
#include "guiao7.h"

int time = 0;
int ctrl_c_counter = 0;

void sig_int_handler(int signum) {
	(void) signum;
	printf("Time elapsed: %d\n", time);
	++ctrl_c_counter;
}

void sig_quit_handler(int signum) {
	(void) signum;
	printf("Times Ctrl+C was pressed: %d\n", ctrl_c_counter);
	_exit(0);
}

void sig_alrm_handler(int signum) {
	(void) signum;
	alarm(1);
	++time;
}

int main() {
	printf("%d\n", getpid());
	signal(SIGINT, sig_int_handler);
	signal(SIGQUIT, sig_quit_handler);
	signal(SIGALRM, sig_alrm_handler);

	alarm(1);

	while (1)
		pause();

	return 0;
}
