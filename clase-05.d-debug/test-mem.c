#include <stdio.h>
#include <malloc.h>

int var_global;			/* var_global: direccion "global" */

int main(void) {
	int var_local;		/* var_local: en el stack */
	static int var_static;	/* var_static: visibilidad "local", direccion "global" */
	void *var_dinamic=malloc(1);	/* var_dinamic: alloc. en el "heap" */

	printf("main        =0x%08x\nvar_global  =0x%08x\nvar_local   =0x%08x\nvar_static  =0x%08x\nvar_dinamic =0x%08x\n",
			&main,
			&var_global,
			&var_local,
			&var_static,
			var_dinamic);
	return 0;
}
