#ifndef _PI_LIB_H
#define _PI_LIB_H

#include <math.h>
#define M_PI		3.14159265358979323846	/* vilmente copiado de math.h */
#define M_PIl		3.1415926535897932384626433832795029L  /* pi long double*/

#define PI M_PIl

/* NOTAr el uso de gcc' long double */
typedef long double pi_double_t;

pi_double_t pi_integra_intervalo(pi_double_t x0, pi_double_t x1, int pasos);
pi_double_t pi_diff(pi_double_t pi_val);

extern int pi_i;

#endif /* _PI_LIB_H */
/* pi_lib.c */
/* pi_lib.c */
