/* 
 * vim:aw:ai:path=.,/usr/include,/usr/include/glib-2.0 
 *
 * Author: JuanJo Ciarlante <jjo-noSPAM@mendoza.gov.ar>
 * License: GPLv2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "palabra.h"
int main(int argc, char **argv) {
	int i;
	char string[64];
	char cmd;
	struct palabra *p;
	FILE *f;
	if (!argv[1])
		{ fprintf(stderr,"uso: %s <archivo_a_leer>\n",argv[0]); return 255;}

	f=fopen(argv[1], "r");
	if (!f) 
		{ perror(argv[1]); return 1;}

	while (fscanf(f, "%63s", string)==1) palabra_add(string);
	close(f);

	while(scanf("%c %63s", &cmd, string)==2) {
		switch(cmd) {
			case 'f':
				p=palabra_find(string);
				if (p)
					printf("cnt=%d string=\"%s\"\n",
							p->cnt, p->string);
				else 
					printf("string=\"%s\" no encontrado\n",
							string);
				break;
		}
	}
	return 0;
}
