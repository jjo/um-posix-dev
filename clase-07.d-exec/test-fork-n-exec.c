/* $Id */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	int status;
	switch(fork()) {
		case -1:	/* error */
			perror("fork()");
			return 1;
		case 0:	/* hijo */
			execlp("ls", "ls", "/al", NULL);
			printf("*** luego del exec()\n");
			return 255;	/* error si llega aqui =) */
	}
	wait (&status);
	printf("*** Saliendo ...\n");
	if (WIFEXITED(status))
		printf("*** status=%d\n", WEXITSTATUS(status));
	return 0;
}

