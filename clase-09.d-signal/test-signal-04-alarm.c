/* 
#define _POSIX_SOURCE 
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handler(int signum) {
	fprintf(stderr, "OUch sig=%d\n", signum);
	alarm(1);
}
int main(void) 
{
	alarm(1);
	signal(SIGALRM, handler);
	getchar();
	return 0;
}
