/* $Id: cluster-0-pi.c,v 1.1 2002/10/10 02:07:55 jjo Exp $ */
/*
 * Objetivo: Cluster MOSIX: ca'lculo de PI con multiproceso
 * Implementacion:
 * 	Se lanza la cantidad de hijos indicada, teniendo cada uno
 * 	de ellos un a'rea distinta; al final el padre suma los 
 * 	resultados de c/hijo.
 *
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 *         Algorithm taken from Professional Linux Programming - WROX
 * 
 *
 *      4  +.
 *         +----+ .
 *         |    +----+ .
 *         |    |    +----+
 *      3  |    |    |    |.
 *         |    |    |    |  . <----- 4/(1+x*x)
 *         |    |    |    |   .
 *         |    |    |    |    
 *      2  |    |    |    |    .
 *         |    |    |    +----+
 *         |    |    |    |    | 
 *         |    |    |    |    | 
 *      1  |    |    |    |    | 
 *         |    |    |    |    | 
 *         |    |    |    |    | 
 *         |    |    |    |    | 
 *      0  +-------------------+
 *         0                   1
 *
 *         <---------><-------->  XXX
 *            hijo1     hijo2     XXX
 * 
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#define __USE_GNU
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define M_PI		3.14159265358979323846	/* vilmente copiado de math.h */
#define M_PIl		3.1415926535897932384626433832795029L  /* pi long double*/

#define PI M_PIl

typedef long double mi_double;
static mi_double __inline__ pi_func(mi_double x)
{
	return (4.0l/(1.0l+x*x));
}
int main (int argc, const char *argv[])
{
	int intervalos;
	int mark;
	int i;
	mi_double x_delta;
	mi_double x, mi_pi;

	if (argc != 2) {
		fprintf(stderr, "uso: %s <cant_intervalos>\n",
				argv[0]);
		return 255;
	}
	intervalos = atoi(argv[1]);
	if (intervalos <=0 || intervalos > 100E6) {
		fprintf(stderr, "ERROR: cantidad de intervalos=%d incorrecta\n",
				intervalos);
		return 255;
	}
	x_delta = 1.0/((double) intervalos);
	x= mi_pi = 0.0;
	mark = intervalos /100;
	for (i=0;i<intervalos;i++) {
		mi_pi += pi_func(x) * x_delta;
		x += x_delta;
		if (i%mark == 0)fprintf(stderr, "x=%6.4Lf\r", x);
	}
	fprintf(stderr, "\n");
	printf("   PI=%20.18Lf\n", PI);
	printf("mi_pi=%20.18Lf error=%20.18Lf (x=%20.18Lf)\n", 
			mi_pi,
			(double) mi_pi - PI,
			x);
	return 0;
}
