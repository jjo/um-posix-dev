/* 
 * $Id: clase-09.d-signal/test-signal-01-catch.c git 2006-05-16 00:14:13.000000000 -0300 jjo Exp $
 */

//#define _POSIX_SOURCE 

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
	signal(SIGALRM, handler); /* deberiamos usar sigaction() */
	alarm(1);
	getchar();
	puts("Saliendo...");
	return 0;
}
