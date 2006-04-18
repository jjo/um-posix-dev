/*
 * $Id: test-fork4.c,v 1.1 2006/04/18 02:26:21 jjo Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *const argv[])
{
	if (fork()==0) return 0;
	sleep(1);
	/* zooOOOmmmbiesss */
	system("ps fx");
	return 0;
}
