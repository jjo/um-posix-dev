/* 
#define _POSIX_SOURCE 
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handler(int signum) {
	fprintf(stderr, "OUch sig=%d\n", signum);
}
int main(void) 
{
	signal(SIGINT, handler); /* deberiamos usar sigaction() */
	getchar();
	return 0;
}
