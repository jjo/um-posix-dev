#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libcifrado.h"

#define BLOCK_SIZE 4096
int main(int argc, char * const argv[])
{
	char inbuf[BLOCK_SIZE];
	char outbuf[BLOCK_SIZE];
	int ilen, olen;
	int do_enc;
	void *ptr;
	char clave[16];

	if (argc!=3) {
		fprintf(stderr, "uso: %s {0|1} laClave\n", argv[0]);
		return 255;
	}
	do_enc=atoi(argv[1]);
	ptr=libcifrado_new();
	memset(clave, 0, sizeof clave);
	strcpy(clave, argv[2]);
	libcifrado_set_clave(ptr, clave, do_enc);

	/*
	 * loop: lee stdin->(de)cifra->escribe stdout 
	 */
	while ((ilen=read (STDIN_FILENO, inbuf, BLOCK_SIZE)) > 0) {
		olen=ilen;
		/*
		 * libcifrado_bloque: cifra desde inbuf[ilen-1] a outbuf[olen-1]
		 */
		if(!(olen=libcifrado_bloque(ptr, inbuf, ilen))) {
			exit(1);
		}
		write(STDOUT_FILENO, outbuf, olen);
	}

	/*
	 * libcifrado_bloque_final: u'ltimo paso de cifrado
	 */
	if (!libcifrado_bloque_final(ptr, outbuf, &olen)) {
		exit(1);
	}
	write(STDOUT_FILENO, outbuf, olen);

	/*
	 * free() del contexto (objeto) de cifrado
	 */
	libcifrado_free(ptr);
	return 0;
}

