/* $Id: cluster-1-pi.c,v 1.1 2002/10/10 20:18:32 jjo Exp $ */
/*
 * Objetivo: Cluster MOSIX: ca'lculo de PI con multiproceso (no aun...)
 * Implementacion:
 * 	Solamente consiste en calcular PI *SIN* nada de multiprocesamiento
 *      
 *                    /~:  x=1
 *                   /           2
 *             pi = /      4/(1+x )  dx
 *                 /
 *              : /
 *               ~  x=0
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 *         Algorithm taken from Professional Linux Programming - WROX
 * 
 *         |
 *      4  +-.
 *         |----+
 *         |//./|
 *         |////|
 *      3  |///.|
 *         |////|  
 *         |////. 
 *         |////| 
 *      2  |////|. <----- 4/(1+x*x)
 *         |////| 
 *         |////|  .
 *         |////+----+           
 *      1  |////|////|.          
 *         |////|////+----+      
 *         |////|////|//./+----+ 
 *         |////|////|////|///.|..
 *      0  +-------------------+------- - -  - 
 *         0                   1
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
#include <signal.h>

#include "pi_lib.h"

enum type {
	VAL_PI,
	VAL_FLOPS
};
struct mi_msg_t {
	enum type type;
	union {
		pi_double_t d;
		int i;
	} val;
};
int child_calc_intervalo(pi_double_t x0, pi_double_t x1, int pasos, int fdout)
{
	struct mi_msg_t msg;
	pi_double_t pi_val_parcial;
	pi_val_parcial = pi_integra_intervalo(x0, x1, pasos);
	msg.type = VAL_PI;
	msg.val.d = pi_val_parcial;
	write (fdout, &msg, sizeof(msg));
	return 0;
}
int main (int argc, const char *argv[])
{
	int hijos;	/* cantidad de hijos a lanzar */
	int tramos;	/* cantidad TOTAL de "divisiones" entre 0 y 1 */
	pi_double_t mi_pi;	/* resultado */
	pi_double_t mi_pi_parcial;	/* resultado de un hijo */
	int pipefd[2];		/* pipe para leer resultado */
	int i;
	pi_double_t x_delta;	/* largo del tramo */
	struct mi_msg_t msg;

	if (argc != 3) {
		fprintf(stderr, "uso: %s <cant_hijos> <cant_tramos>\n",
				argv[0]);
		return 255;
	}
	hijos  = atoi(argv[1]);
	tramos = atoi(argv[2]);
	if (tramos <10 || tramos > 100E6) {
		fprintf(stderr, "ERROR: cantidad de tramos=%d incorrecta\n",
				tramos);
		return 255;
	}
	if (hijos <=0  || hijos > 100) {
		fprintf(stderr, "ERROR: cantidad de hijos=%d incorrecta\n",
				hijos);
		return 255;
	}
	if (tramos % hijos) {
		fprintf(stderr, "ERROR: cantidad de tramos(%d) debe ser divisible por la cantidad de hijos(%d)\n",
				tramos,
				hijos);
		return 255;
	}
	if (pipe(pipefd)<0) {
		perror("pipe()");
		return 1;
	}
	x_delta = 1.0/hijos;
	signal(SIGCHLD, SIG_IGN);
	for (i=0; i < hijos; i++) {
		switch(fork()) {
			case 0:	/* hijo */
				close(pipefd[0]);
				return child_calc_intervalo(x_delta*i, x_delta*(i+1), tramos/hijos, pipefd[1]);
				/* NOT_REACHED */
			case -1:
				perror("fork()");
				return 2;
			default: /* padre */
				break;
		}
	}
	close(pipefd[1]);
	mi_pi = 0.0;
	while( read(pipefd[0], &msg, sizeof(msg))>0 ) {
		switch (msg.type) {
			case VAL_PI: mi_pi += msg.val.d; break;
			default: break;
		}
	}
	fprintf(stderr, "\n");
	printf("   PI=%20.18Lf\n", PI);
	printf("mi_pi=%20.18Lf error=%20.18Lf\n",
			mi_pi,
			pi_diff(mi_pi));
	return 0;
}
