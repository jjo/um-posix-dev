//#define _SVID_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

extern int errno;
void handler(int signum) {
	fprintf(stderr, "OUch sig=%d\n", signum);
}
int main(void) 
{
	char ch;
	alarm(1);
	signal(SIGALRM, handler);
	if (read(0, &ch, 1)<0)
		fprintf(stderr, "errno=%d (%s)\n", errno, strerror(errno));
	return 0;
}
