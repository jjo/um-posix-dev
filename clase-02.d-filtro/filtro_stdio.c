/*
 * 	clase-02: separacion entre implementacion e interfase
 *
 * 	filtro.c: encargado de procesar el stream
 * 	convertir.c: solo convierte
 *
 * 	Autor: JuanJo Ciarlante <jjo@um.edu.ar>
 *	Licencia: GPL2 (no usar en proyectos sin fuentes publicos)
 * 	
 */
#include <stdio.h>
#include "convertir.h"

int main(void) {
	char buf[1024];
	int nread;

	while((nread=fread(buf, 1, sizeof(buf), stdin))>0) {
		convertir(buf, nread);
		fwrite(buf, 1, nread, stdout);
	}
	return 0;
}
