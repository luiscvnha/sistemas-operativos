#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    int pid, exec_ret, status;

    if ((pid = fork()) == 0) {
    	exec_ret = execlp("ls", "ls", "-l", NULL);
    	perror("reached return");
    	_exit(exec_ret);
    } else {
    	wait(&status);

    	if (WIFEXITED(status))
    		printf("O filho retornou %d\n", WEXITSTATUS(status));
    	else
    		printf("O filho não terminou\n");
    }

    return 0;
}
