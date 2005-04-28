#include <stdio.h>
#include <fcntl.h>	/* open() */
#include <sys/stat.h>	/* umask() */
#include "libopen.h"

/**
 *  Invoca open con flags=0, mode=0
 */
static int open_1_cero(const char *fname)
{
	fprintf(stderr, "-> open(\"%s\", 0, 0)\n", fname);
	return open (fname, 0, 0);
}
/**
 *  Invoca open con flags=O_CREAT, mode=0
 */
static int open_2_creat(const char *fname)
{
	fprintf(stderr, "-> open(\"%s\", O_CREAT, 0)\n", fname);
	int fd= open (fname, O_CREAT|O_RDWR, 0);
	write(fd, "hola", 5);
	return fd;
}
/**
 *  Invoca open con flags=O_CREAT, mode=0666
 */
static int open_3_creat_mode(const char *fname)
{
	fprintf(stderr, "-> open(\"%s\", O_CREAT, 0666)\n", fname);
	return open (fname, O_CREAT, 0666);
}
/**
 *  Invoca open con flags=O_CREAT, mode=0666 pero con umask(0) previo
 */
static int open_4_creat_mode_umask(const char *fname)
{
	umask(0000);
	fprintf(stderr, "-> umask(0000);open(\"%s\", O_CREAT, 0666)\n", fname);
	return open (fname, O_CREAT, 0666);
}

/**
 * Define un tipo de  _funcion_ que recibe un
 * argumento (const char *) y devuelve int
 */
typedef int (open_func_t)(const char *);

/**
 * Array de funciones para mostrar distintos comportamientos
 * de open()
 */
static open_func_t *open_array[] = {
	open_1_cero,
	open_2_creat,
	open_3_creat_mode,
	open_4_creat_mode_umask
};

/**
 * Forma generica de devolver la cantidad de elementos
 * de un array:   sizeof array/sizeof elem
 */
#define OPEN_ARRAY_SIZE ( sizeof open_array/sizeof open_array[0])

/**
 * Funcion de entrada para invocar "alguna" de las funciones
 * open_X_..., dependiendo de num
 *
 * @param num numero de funcion open a invocar
 * @param  fname nombre del archivo a abrir
 * @return el retorno de open(...)
 */
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

