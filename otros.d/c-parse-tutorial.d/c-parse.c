/* 
 * mini parser a modo de tutorial de scanf(): lee ./input.txt que
 * debe estar formado por lineas del tipo:
 *           variable = valor
 * 
 * Autor: Juanjo Ciarlante <jjo@mendoza.gov.ar>
 * Licencia: BSD
 */

#include <stdio.h>
#include <stdlib.h>

#define P_VAR "[^# =]"	/* patt. para nombre de variable */
#define P_EQU "[ =]"	/* patt. para el '=' y sus espacios alrededor */
#define P_VAL "[^ \n]"  /* patt. para el valor de la var.*/

int main(void)
{
	char buf[256];
	char var[32], val[32];
	int ret;
	FILE *f=fopen("input.txt","r");
	if (!f) { perror("fopen");exit (1); }

	while(fgets(buf, sizeof(buf) -1, f)) {
		var[0]=0; val[0]=0;
		ret=sscanf(buf, " %31" P_VAR "%*31" P_EQU "%31" P_VAL
				, var, val);
		printf("READ:%s", buf);
		/* solamente sirve si ret==2 */
		printf("PARS:%s=%s (ret=%d)\n\n", 
				var, val, ret);
	}
	return 0;
}
