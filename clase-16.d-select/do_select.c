#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define tostr(x) #x
#define ERRSYS(call) do { if ( (call) < 0) { perror(tostr(call));exit(1); }} while (0)
int conecta_a(const char *str, unsigned port)
{
	int sockfd;
	struct sockaddr_in s_in;
	ERRSYS( sockfd=socket(PF_INET, SOCK_STREAM, 0) );
	s_in.sin_family = AF_INET;
	s_in.sin_port = htons(port);
	if (inet_aton(str, &s_in.sin_addr)==0) {
		fprintf(stderr, "inet_aton(\"%s\", ...) fallo'\n", str);
		exit(1);
	}
	ERRSYS( connect (sockfd, (struct sockaddr *)&s_in, sizeof(s_in)) );
	return sockfd;
}
int main(void) {
	int sockfd;
	fd_set rfds, wfds, efds;
	struct timeval tv; 
	char buf[1024];
	int nread;
	int nselect;

	sockfd=conecta_a ("64.157.4.82", 25);
	while (1) {
		/* setear los descriptores para CADA loop */
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);

		/* espero "ready to read" en red y stdin */
		FD_SET(sockfd,       &rfds);
		FD_SET(STDIN_FILENO, &rfds);

		/* espero "excepciones" en red y stdin */
		FD_SET(sockfd,       &efds);
		FD_SET(STDIN_FILENO, &efds);

		/* recargo timer para CADA loop en 5segs*/
		tv.tv_sec = 5;
		tv.tv_usec= 0;

		/* llamada a select() */
		ERRSYS( nselect=select (1024, &rfds, &wfds, &efds, &tv) );

		/* nselect==0 => ningu'n descriptor listo: timeout */
		if (nselect==0) {
			fprintf(stderr, "** TICK **\n");
			continue;
		}
		if (FD_ISSET(STDIN_FILENO, &efds)) {
			fprintf(stderr, "** stdin EXCEPTION\n");
			break;
		}
		if (FD_ISSET(sockfd, &efds)) {
			fprintf(stderr, "** red EXCEPTION\n");
			break;
		}
		/* ver cual descriptor esta' listo ... */
		if (FD_ISSET(STDIN_FILENO, &rfds)) {
			ERRSYS( nread=read(STDIN_FILENO, buf, sizeof(buf)) );
			if (nread==0) { fprintf(stderr, "stdin: EOF\n"); break; }
			fprintf(stderr, "** stdin -> red\n");
			ERRSYS( write(sockfd, buf, nread) );
		}
		if (FD_ISSET(sockfd, &rfds)) {
			ERRSYS( nread=read(sockfd, buf, sizeof(buf)) );
			if (nread==0) { fprintf(stderr, "red: EOF\n"); break; }
			fprintf(stderr, "** red -> stdout\n");
			ERRSYS( write(STDOUT_FILENO, buf, nread) );
		}

	}
	return 0;
}
