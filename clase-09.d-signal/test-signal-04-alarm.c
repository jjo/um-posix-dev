/* 
 * $Id: test-signal-04-alarm.c,v 1.4 2003/05/16 21:13:50 jjo Exp $
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
	alarm(1);
}
int main(void) 
{
	alarm(1);
	signal(SIGALRM, handler); /* deberiamos usar sigaction() */
	getchar();
	return 0;
}
