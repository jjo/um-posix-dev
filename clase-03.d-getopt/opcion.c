/*
 * 	$Id: opcion.c,v 1.2 2003/03/21 02:48:59 jjo Exp $
 *
 * 	Ejemplo de uso de getopt() 
 * 	Autor: JuanJo Ciarlante
 * 	Licencia: GPLv2
 *
 *
 * Usos:
 * 	./opcion
 * 	./opcion -b
 * 	./opcion -i 12345
 * 	./opcion -s algoaqui
 * 	./opcion -b -i 12345 -s algoaqui
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Ver getopt(3) */
extern char *optarg;
extern int optind, opterr, optopt;

char *program_name;

int opt_b = 0;		/* -b           : opcion 'booleana' (flag) */
int opt_i = -1;		/* -i <entero>  : opcion integer */
char * opt_s = NULL;	/* -s <string>  : opcion string */

int main(int argc, char * const argv[])
{
	int c;
	program_name=argv[0];
	/* printf("argc=%d argv1=%s\n", argc, argv[1]); */

	/* bucle para loopear por las opciones pasadas */
	while ((c=getopt(argc, argv, "bi:s:"))>=0) {
		switch (c) {
			case 'b':	/* -b : flag */
				opt_b++;
				break;
			case 'i':	/* -i <entero> */
				opt_i=atoi(optarg); 
				break;
			case 's':	/* -s <string> */
				opt_s=optarg;
				break;
			default:
				fprintf(stderr, "opcion invalida: %c\n", c);
				break;
		}
	}
	/*
	printf("optind=%d\n", optind);
	argc-=(optind-1);
	argv+=(optind-1);
	printf("argc=%d argv1=%s\n", argc, argv[1]);
	*/
	printf("opcion b=%d\n", opt_b);
	printf("opcion i=%d\n", opt_i);
	printf("opcion s=%s\n", opt_s ? opt_s : "<NULL>");
	return 0;
}
