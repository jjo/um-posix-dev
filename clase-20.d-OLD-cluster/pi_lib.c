#include <stdio.h>
#include "pi_lib.h"
static pi_double_t __inline__ pi_func(pi_double_t x)
{
	return (4.0l/(1.0l+x*x));
}

int pi_i = 0;
pi_double_t pi_integra_intervalo(pi_double_t x0, pi_double_t x1, int pasos)
{
	/* int i; */
	pi_double_t x, x_delta;
	pi_double_t pi_val = 0.0;
	int marca = pasos/100;
	if (marca==0) marca = 1;

	x_delta = (x1 - x0)/pasos;
	x = x0;
	for (pi_i=0; pi_i<pasos ; pi_i++) {
		pi_val += pi_func(x) * x_delta;
		x += x_delta;
		/* if (i%marca == 0)fprintf(stderr, "x=%6.4Lf\r", x); */
	}
	return pi_val;
}
pi_double_t pi_diff (pi_double_t pi_val)
{
	return pi_val - PI;
}
