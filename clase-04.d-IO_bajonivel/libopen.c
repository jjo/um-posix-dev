#include <stdio.h>
#include <fcntl.h>	/* open() */
#include <sys/stat.h>	/* umask() */

#include "libopen.h"
static int open_1_cero(const char *fname)
{
	fprintf(stderr, "-> open(\"%s\", 0, 0)\n", fname);
	return open (fname, 0, 0);
}
static int open_2_creat(const char *fname)
{
	fprintf(stderr, "-> open(\"%s\", O_CREAT, 0)\n", fname);
	return open (fname, O_CREAT, 0);
}
static int open_3_creat_mode(const char *fname)
{
	fprintf(stderr, "-> open(\"%s\", O_CREAT, 0666)\n", fname);
	return open (fname, O_CREAT, 0666);
}
static int open_4_creat_mode_umask(const char *fname)
{
	umask(0000);
	fprintf(stderr, "-> umask(0777);open(\"%s\", O_CREAT, 0666)\n", fname);
	return open (fname, O_CREAT, 0666);
}

typedef int (open_func_t)(const char *);
static open_func_t *open_array[] = {
	open_1_cero,
	open_2_creat,
	open_3_creat_mode,
	open_4_creat_mode_umask
};
#define OPEN_ARRAY_SIZE ( sizeof open_array/sizeof *open_array)
int open_test(int num, const char *fname)
{
	open_func_t *open_func;
	if (num <= 0 && num > OPEN_ARRAY_SIZE)
	{
		fprintf(stderr, "ERROR: numero=%d incorrecto\n", num);
		return 1;
	}
	open_func=open_array[num-1];
	return open_func(fname);
}

