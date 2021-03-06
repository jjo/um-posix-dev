/* $Id */
#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

int main(void) {
	int status;
	pid_t pid_hijo;
	int fd;
	if ((fd=open("Makefile", O_RDONLY, 0))< 0) {
		perror("Makefile");
		return 1;
	}
	if (dup2(fd, STDIN_FILENO)<0) {
		perror("dup2()");
		return 2;
	}
	switch(fork()) {
		case -1:	/* error */
			perror("fork()");
			return 1;
		case 0:	/* hijo */
			execlp("cat", "cat", (char*)NULL);
			printf("*** luego del exec()\n");
			return 255;	/* error si llega aqui =) */
	}
	pid_hijo=wait (&status);
	printf("*** Saliendo (pid_hijo=%d)...\n", pid_hijo);
	if (WIFEXITED(status))
		printf("*** status=%d\n", WEXITSTATUS(status));
	return 0;
}

