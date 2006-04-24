/*
 * $Id: test-fork4.c,v 1.2 2006/04/24 20:49:10 jjo Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *const argv[])
{
	if (fork()==0) return 0;
	if (fork()==0) return 0;
	sleep(1);
	/* zooOOOmmmbiesss */
	system("ps fx");
	return 0;
}
