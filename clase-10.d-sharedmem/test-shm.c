/* $Id: test-shm.c,v 1.4 2004/08/20 21:22:13 jjo Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>	/* IPC: parte en comun */
#include <sys/shm.h>	/* shared mem */
#include <time.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>

#define MEMSIZE   16384
const char *progname;
char *shm_abre_y_attachea(key_t key, size_t size, mode_t mode)
{
	static int shm_id=0;
	char *ptr;
	fprintf(stderr, "Usando    key=0x%08x\n", key);

	/* 
	 * abre el segmento de memoria compartida, crea'ndolo
	 * si no existiera
	 */
	shm_id=shmget(key, MEMSIZE, mode);
	fprintf(stderr, "Usando shm_id=%d\n", shm_id);
	if (shm_id < 0)
		return NULL;

	/*
	 * hace un "attach" del segmento de memoria compartida
	 * dentro del espacio de memoria del proceso
	 */
	ptr=shmat(shm_id, NULL, 0);
	return ptr;
}
void usage(FILE *f, int exit_status) {
	fprintf(f, "ERROR: uso %s {-r|-w} id\n", progname);
	exit(exit_status);
}
int main(int argc, char * const argv[])
{
	key_t key;
	int c;
	char *ptr=NULL;
	const char *ouch=NULL;
	int opt_read=0, opt_write=0;
	int count=0;
	
	progname=argv[0];
	while((c=getopt(argc, argv, "rw"))!=-1) {
		switch(c) {
			case 'r': opt_read++;break;
			case 'w': opt_write++;break;
			default:  usage(stderr, 255);break;
		}

	}
	if (! ( opt_read ^ opt_write) ) usage(stderr, 255);
	argc-=(optind-1);
	argv+=(optind-1);

	if (argc!=2) usage(stderr, 255);
	key=atoi(argv[1]);
	if (key<0)   { ouch="key invalido";goto err;}

	ptr=shm_abre_y_attachea(key, MEMSIZE, 0666 | (opt_write? IPC_CREAT : 0));
	if (!ptr)    { ouch="obtener memoria compartida";goto err;}
	
	if (opt_write) {
		/* opcion -w: escribe en memoria */
		int resto=MEMSIZE-1;	/* lugar para el '\0' */
		fprintf(stderr, "Leyendo de stdin ...resto\n");
		/* 
		 * consume el stdin, dejando su contenido
		 * DIRECTAMENTE en ptr
		 */
		while( (count=read(STDIN_FILENO, ptr, resto)) >0 ) {
			resto-=count;
			if (resto<0) {
				fprintf(stderr, "*** ERROR: se acabo' el espacio\n");
				break;
			}
			ptr+=count;
		}
		if (count<0) {ouch="read()"; goto err;}
		/* soy un lamer: caracter de fin de string :P */
		ptr[count]='\0';
		
	} else if(opt_read) {
		/* opcion -r: lee desde memoria */
		fprintf(stderr, "Escribiendo en stdout ...\n");
		puts(ptr);
	}
	return 0;

err:
	fprintf(stderr, "ERROR: %s: %s\n", ouch, strerror(errno));
	return 1;
}
