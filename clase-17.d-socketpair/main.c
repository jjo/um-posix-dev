/* Author: JuanJo Ciarlante <jjo@um.edu.ar> */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#define ERRSYS(call) do { if ( (call) < 0) { perror(#call);exit(1); }} while (0)

#include "calc.h"
int calc_client_interactive(int fd) {
	char buf[256];
	char op_char;
	char *line;
	int a,b,ret;
	while ((line=fgets(buf, sizeof buf, stdin))) {
		if (sscanf(line, "%c %d %d\n", &op_char, &a, &b)==3) {
			switch(op_char) {
				case 's': /* suma */
					ret=calc_client_integer(fd, SUMAR, a, b);
					printf("resultado=%d\n", ret);
					break;
				default: 
					fprintf(stderr, "operacion '%c' no contemplada\n", op_char);
			}
		} else { 
			fprintf(stderr, "ERROR: %s\n", line);
		}
	}
	return 0;
}
int main(void)
{
	int fds[2];
	ERRSYS(socketpair(PF_UNIX, SOCK_DGRAM, 0, fds));
	signal(SIGCHLD, SIG_IGN);
	switch(fork()) {
		case 0: /* hijo */
			close(fds[0]);
			return calc_server(fds[1]);
		case -1: /* error*/
			perror("fork()");
			return 1;
	}
	/* padre */
	close(fds[1]);
	calc_client_interactive(fds[0]);
	return 0;
}
