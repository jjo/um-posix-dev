/* 
#define _POSIX_SOURCE 
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handler(int signum) {
	fprintf(stderr, "OUch sig=%d\n(poniendo a DFL)\n", signum);
	signal (signum, SIG_DFL);
}
int main(void) 
{
	signal(SIGINT, handler);
	getchar();
	return 0;
}
