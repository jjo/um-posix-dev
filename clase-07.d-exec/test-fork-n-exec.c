/* $Id: test-fork-n-exec.c,v 1.3 2006/04/24 20:53:04 jjo Exp $ */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	int status;
	pid_t pid_hijo;
	switch(fork()) {
		case -1:	/* error */
			perror("fork()");
			return 1;
		case 0:	/* hijo */
			execlp("ls", "ls", "-al", NULL);
			printf("*** luego del exec()\n");
			return 255;	/* error si llega aqui =) */
	}
	pid_hijo=wait (&status);
	printf("*** Saliendo (pid_hijo=%d)...\n", pid_hijo);
	if (WIFEXITED(status))
		printf("*** status=%d\n", WEXITSTATUS(status));
	return 0;
}

