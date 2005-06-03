/* $Id: test-filedes.c,v 1.3 2005/06/03 17:36:15 jjo Exp $ */
/*
 * Objetivo: Mostrar la utilizacio'n de descriptores de archivo (fd) mediante
 *           open(), dup(), dup2()
 * 
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

/**  Nombre del  "ejecutable", es decir: argv[0] pasado a main() */
static const char *progname = NULL;

static int  ejecuto_cmd(const char *cmd);
static void usage(FILE *f);
int main(int argc, char * const argv[])
{
	char *fname, *cmd;
	int fd;
	int fdbak;

	progname = argv[0];
	if (argc != 3) {
		usage(stderr);
		return 1;
	}
	fname = argv[1];
	cmd = argv[2];

	fd=open(fname, O_CREAT|O_RDWR, 0666);
	if (fd<0) {
		perror(fname);
		return 255;
	}
	printf("fd=%d\n", fd);

	/* "Backupeamos" en otro descriptor el actual stdout */
	if ((fdbak=dup(STDOUT_FILENO)) < 0) {
		perror("dup()");
		return 2;
	}

	/* Hacemos que el descriptor stdout (fd==1) "apunte" al nuevo fd */
	if (dup2(fd, STDOUT_FILENO)<0) {
		perror("dup2(fd)");
		return 2;
	}

	ejecuto_cmd(cmd);

	/* "Restauramos" el stdout anterior */
	if (dup2(fdbak, STDOUT_FILENO) < 0) {
		perror("dup2(fdbak)");
		return 2;
	}
	printf("Es e'ste mi standard output original ? \n");
	return 0;
}
/** 
 * Ejecuto "cmd" con el objetivo de "generar" algo en stdout
 * @param cmd comando a ejecutar, por ej: "ls -al"
 * @return el status de terminacion de system()
 */
int ejecuto_cmd(const char *cmd) 
{
	time_t curtime;		/* tiempo actual */
	int status; 		/* status de terminacion del system() */
	time(&curtime);
	printf("* COMIENZO -- %s\n", ctime(&curtime));
	status=system(cmd);
	time(&curtime);
	printf("* FIN      -- %s\n", ctime(&curtime));
	return status;
}
static void usage(FILE *f)
{
	fprintf(f, "uso: %s <archivo> \"<comando ... argumentos>\"\n", progname);
}
