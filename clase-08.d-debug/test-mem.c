#include <stdio.h>
#include <malloc.h>

int var_global;			/* var_global: direccion "global" */
#define MB (1024*1024)

int main(void) {
	/* var_static: visibilidad "local", direccion "global" */
	static int var_static;	

	/* var_local: en el stack */
	int var_local;		

	/* var_dinamic: alloc. en el "heap" */
	void *var_din1=malloc(100*MB);
	void *var_din2=malloc(100*MB);
	void *var_din3=malloc(100*MB);

	printf( "main        =0x%08x   (%6u Mb)\n"
		"var_global  =0x%08x   (%6u Mb)\n"
		"var_static  =0x%08x   (%6u Mb)\n"
		"var_din1    =0x%08x   (%6u Mb)\n"
		"var_din2    =0x%08x   (%6u Mb)\n" 
		"var_din3    =0x%08x   (%6u Mb)\n" 
		"var_local   =0x%08x   (%6u Mb)\n"
		,
			(unsigned) &main       ,(unsigned) &main       /MB,
			(unsigned) &var_global ,(unsigned) &var_global /MB,
			(unsigned) &var_static ,(unsigned) &var_static /MB,
			(unsigned)  var_din1   ,(unsigned)  var_din1   /MB,
			(unsigned)  var_din2   ,(unsigned)  var_din2   /MB,
			(unsigned)  var_din3   ,(unsigned)  var_din3   /MB,
			(unsigned) &var_local  ,(unsigned) &var_local  /MB);
	return 0;
}
