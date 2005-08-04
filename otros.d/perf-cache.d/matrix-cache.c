/* $Id: matrix-cache.c,v 1.2 2005/08/04 15:05:37 jjo Exp $ */
/* Muestra la diferencia de velocidad entre loop externo por filas o columnas,
 * causada por un buen/mal uso de cache
 */
#define COLS 10000
#define ROWS 1000
int a[ROWS][COLS];
int b[ROWS][COLS];
int c[ROWS];
int main(void) {
	int i,j;
#if   defined( CACHE_BAD )
	for (j = 0; j < COLS; j++)
		for (i = 0; i < ROWS; i++)
#elif defined( CACHE_GOOD )
    	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLS; j++)
#else
#error "Por favor compilar con -DCACHE_BAD o -DCACHE_GOOD" 
#endif
		    a[i][j] = b[i][j] + c[i];
	return 0;
}
