#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "libopen.h"

/**  Nombre del  "ejecutable", es decir: argv[0] pasado a main() */
const char *progname;

void help(void)
{
	fprintf(stderr, "ERROR: uso %s <0-3> <archivo> \n", progname);
}
int main(int argc, char * const argv[])
{
	int fd;			/* file descriptor */
	int numero_de_func;	/* nro de funcion a invocar */
	const char *fname; 	/* nombre de archivo a abrir */
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
