#include <unistd.h>
#include <fcntl.h>	/* open() */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>	/* umask() */

const char *progname;	/* nombre del  "ejecutable" */

int open_1_cero(const char *fname)
{
	fprintf(stderr, "-> open(\"%s\", 0, 0)\n", fname);
	return open (fname, 0, 0);
}
int open_2_creat(const char *fname)
{
	fprintf(stderr, "-> open(\"%s\", O_CREAT, 0)\n", fname);
	return open (fname, O_CREAT, 0);
}
int open_3_creat_mode(const char *fname)
{
	fprintf(stderr, "-> open(\"%s\", O_CREAT, 0666)\n", fname);
	return open (fname, O_CREAT, 0666);
}
int open_4_creat_mode_umask(const char *fname)
{
	umask(0000);
	fprintf(stderr, "-> umask(0777);open(\"%s\", O_CREAT, 0666)\n", fname);
	return open (fname, O_CREAT, 0666);
}

typedef int (open_func_t)(const char *);
open_func_t *open_array[] = {
	open_1_cero,
	open_2_creat,
	open_3_creat_mode,
	open_4_creat_mode_umask
};
#define OPEN_ARRAY_SIZE ( sizeof open_array/sizeof *open_array)
void help(void)
{
	fprintf(stderr, "ERROR: uso %s <0-3> <archivo> \n", progname);
}
int main(int argc, char * const argv[])
{
	int fd;		/* file descriptor */
	int numero_de_func;
	const char *fname;
	open_func_t *open_func;
	progname = argv[0];

	if (argc != 3) {
		help();
		return 255;
	}

	numero_de_func=atoi(argv[1]);
	fname=argv[2];
	if (numero_de_func <= 0 && numero_de_func > OPEN_ARRAY_SIZE)
	{
		fprintf(stderr, "ERROR: numero=%d incorrecto\n", numero_de_func);
		return 1;
	}
	open_func=open_array[numero_de_func-1];
	fd = open_func(fname);
	if (fd < 0) 
	{
		perror("open");
		return 2;
	}
	return 0;
}
