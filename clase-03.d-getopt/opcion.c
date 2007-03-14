/*
 * 	$Id: opcion.c,v 1.3 2003/03/28 18:29:59 jjo Exp $
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

int opcion_b = 0;		/* -b           : opcion 'booleana' (flag) */
int opcion_i = 0;		/* -i <entero>  : opcion integer */
char * opcion_s = NULL;	/* -s <string>  : opcion string *//*@null@*/

int main(int argc, char * const argv[])
{
	int c;
	program_name=argv[0];
	/* printf("argc=%d argv1=%s\n", argc, argv[1]); */

	/* bucle para loopear por las opciones pasadas */
	while ((c=getopt(argc, argv, "bi:s:"))>=0) {
		switch (c) {
			case 'b':	/* -b : flag */
				opcion_b++;
				break;
			case 'i':	/* -i <entero> */
				opcion_i=atoi(optarg);
				/* 
				 * en este ejemplo, consideramos valor 0
				 * inva'lido
				 */
				if (opcion_i == 0) {
					fprintf(stderr, "ERROR: valor de '-i' invalido\n");
					return 1;
				}
				break;
			case 's':	/* -s <string> */
				opcion_s=optarg;
				break;
			default:
				fprintf(stderr, "opcion invalida: %c\n", c);
				return 1;
		}
	}
	/*
	printf("optind=%d\n", optind);
	argc-=(optind-1);
	argv+=(optind-1);
	printf("argc=%d argv1=%s\n", argc, argv[1]);
	*/
	printf("opcion b=%d\n", opcion_b);
	printf("opcion i=%d\n", opcion_i);
	printf("opcion s=\"%s\"\n", opcion_s ? opcion_s : "<NULL>");
	return 0;
}
