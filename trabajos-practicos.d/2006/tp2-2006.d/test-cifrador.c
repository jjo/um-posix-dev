#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libcifrado.h"

#define BLOCK_SIZE 4096
int main(int argc, char * const argv[])
{
	char buf[BLOCK_SIZE];
	int ilen, olen;
	int do_enc;
	void *ptr;

	if (argc!=3) {
		fprintf(stderr, "uso: %s {0|1} laClave\n", argv[0]);
		return 255;
	}
	do_enc=atoi(argv[1]);
	ptr=libcifrado_new();
	libcifrado_set_clave(ptr, argv[2], strlen(argv[2]), do_enc);

	/*
	 * loop: lee stdin->(de)cifra->escribe stdout 
	 */
	while ((ilen=read (STDIN_FILENO, buf, BLOCK_SIZE)) > 0) {
		olen=ilen;
		/*
		 * libcifrado_bloque: cifra buf (SIMPLIFICADO)
		 */
		if((olen=libcifrado_bloque(ptr, buf, ilen))==0) {
			exit(1);
		}
		write(STDOUT_FILENO, buf, olen);
	}

	/*
	 * free() del contexto (objeto) de cifrado
	 */
	libcifrado_free(ptr);
	return 0;
}

