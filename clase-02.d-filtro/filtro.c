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
#include <unistd.h>
#include "convertir.h"

int main(void) {
	char buf[1024];
	int nread;

	while((nread=read(0, buf, sizeof(buf)))>0) {
		convertir(buf, nread);
		write(1, buf, nread);
	}
	return 0;
}
