/* 
 * $Id: test-signal-05-sigaction.c,v 1.2 2003/05/16 21:13:50 jjo Exp $
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handler(int signum) {
	fprintf(stderr, "OUch sig=%d\n", signum);
}
void handler_siginfo(int signum, siginfo_t *si, void *data) {
	const char *who;
	switch(si->si_code) { 
		case SI_USER: who="user";break;
		case SI_KERNEL: who="kernel";break;
		default: who="<nocontemplado>";break;
	}
	fprintf(stderr, "OUch sig=%d si_code=%d who=%s\n",
			si->si_signo, si->si_code, who);
	
}
int main(void) 
{
	struct sigaction sa;
#if 1
	sa.sa_flags= SA_RESTART;
	sa.sa_handler=handler;
#else
	sa.sa_flags= SA_RESTART|SA_SIGINFO;
	sa.sa_sigaction = handler_siginfo;
#endif
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	getchar();
	return 0;
}

