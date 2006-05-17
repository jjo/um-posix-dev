/* $Id: test-fork-n-exec.c,v 1.4 2006/05/01 21:57:24 jjo Exp $ */
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
			//kill(getpid(),9);
			execlp("ls", "ls", "-al", (char*)NULL);
			printf("*** luego del exec()\n");
			return 255;	/* error si llega aqui =) */
	}
	pid_hijo=wait (&status);
	printf("*** Saliendo (pid_hijo=%d)...\n", pid_hijo);
	if (WIFEXITED(status))
		printf("*** status=%d\n", WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("*** signaled signal=%d\n", WTERMSIG(status));

	return 0;
}

