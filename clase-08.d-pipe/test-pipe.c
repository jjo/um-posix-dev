#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/* 
 * Este ejemplo falla (a propo'sito, claro esta')  
 * porque el padre se queda "esperando" ... porque?
 */
int main(void) {
	int fds[2];
	int nread; 
	char buf[256];

	if (pipe(fds) < 0) {
		perror("pipe");return 255;
	}
	if (fork()==0) {
		/* hijo */
		/* escribo en el lado escritura (en que' otro sino? ) */
		write(fds[1], "ahi vaaaa ", 10);
		usleep(1);
		write(fds[1], "012345678 ", 10);
		return 0;
	}
	/* leo en el lado lectura ... */
	while((nread=read(fds[0], buf, sizeof(buf)-1)) > 0) {
		buf[nread-1]=0;
		printf("buf=%s, nread=%d\n", buf, nread);
	}
	wait(NULL);
	return 0;
}
