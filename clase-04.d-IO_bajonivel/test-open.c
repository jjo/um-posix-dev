#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "libopen.h"
const char *progname;	/* nombre del  "ejecutable" */

void help(void)
{
	fprintf(stderr, "ERROR: uso %s <0-3> <archivo> \n", progname);
}
int main(int argc, char * const argv[])
{
	int fd;		/* file descriptor */
	int numero_de_func;
	const char *fname;
	progname = argv[0];

	if (argc != 3) {
		help();
		return 255;
	}

	numero_de_func=atoi(argv[1]);
	fname=argv[2];
	fd = open_test(numero_de_func, fname);
	if (fd < 0) 
	{
		perror("open");
		return 2;
	}
	return 0;
}
