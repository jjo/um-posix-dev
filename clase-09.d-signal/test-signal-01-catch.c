/* 
 * $Id: test-signal-01-catch.c,v 1.4 2003/05/16 21:13:50 jjo Exp $
 */

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
