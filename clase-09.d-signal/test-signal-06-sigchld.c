#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

/* variable global para poder leerla en el handler */
int i;

/* funcion simple para lanzar un hijo ... */
void lanza_hijo(void)
{
	if (fork()==0)
	{
		printf("hijo corriendo, pid=%d\n", getpid());
		sleep(2);	/* vivo este tiempo ... */
		exit(0);	/* fin del hijo */
	}
	/* padre regresa sin hacer nada mas */
}
void handler(int sig)
{
	char buf[256];
	int len;
	pid_t pidhijo;
	while (( pidhijo=waitpid(-1, NULL, 0)) > 0) {	/* "leo" la muerte del hijo */
		/* evito usar printf() o fprintf() a traves de un bufer local */
		len=snprintf(buf, sizeof buf, " [i=%d pid=%d]", i, pidhijo);
		write(2, buf, len);
	}
	write(2,"\n", 1);

	/* lanzo otro hijo ... */
	lanza_hijo();		
}
const char *progreso=")O(|";
int main(void) 
{
	struct sigaction sigact;
	/* Preparacion de la struct sigaction */
	sigact.sa_handler=handler;
	sigact.sa_flags=0;
	sigemptyset(&sigact.sa_mask);

	/* instalacion del handler */
	sigaction(SIGCHLD, &sigact, NULL);
	lanza_hijo();
	/* loop "principal" del programa */
	for (i=0;i<1000;i++) {
		write(2, "\r", 1);
		write(2, &progreso[i%4], 1);
		usleep(90000);
	}
	return 0;
}
