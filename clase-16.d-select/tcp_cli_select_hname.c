#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>		/* resolucion de nombres */
#define ERRSYS(call) do { if ( (call) < 0) { perror(#call);exit(1); }} while (0)

int fprintf_ip(FILE *f, struct sockaddr_in *addr_in) {
	char ip_buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, (void*)&(addr_in->sin_addr), ip_buf, sizeof(ip_buf));
	return fprintf(stderr, "IP=%s\n", ip_buf);
}
/* emula inet_aton *pero* permitiendo resolver nombres -> ip*/
int inet_aton_hname(const char *cp, struct in_addr *inp)
{

	const struct addrinfo hint = 
		{ .ai_flags=0, .ai_family=AF_INET, .ai_socktype=0, .ai_protocol=0 };
	struct addrinfo *addrinfo_p;

	/* Usamos getaddrinfo en vez del (obsoleto) gethostbyname */
	if ((getaddrinfo(cp, NULL, &hint, &addrinfo_p))==0) {
		struct sockaddr_in *addr_in=(struct sockaddr_in*)addrinfo_p->ai_addr;
		memcpy(inp, &addr_in->sin_addr, sizeof (addr_in->sin_addr));

		return 1;
	} else {
		fprintf(stderr, "error en gethostbyname(\"%s\"): %s\n",
				cp,
				hstrerror(h_errno));
	}
	return 0;
}
int conecta_a(const char *str, unsigned port)
{
	int sockfd;
	struct sockaddr_in s_in;
	ERRSYS( sockfd=socket(PF_INET, SOCK_STREAM, 0) );
	s_in.sin_family = AF_INET;
	s_in.sin_port = htons(port);
	if (inet_aton_hname(str, &s_in.sin_addr)==0) {
		fprintf(stderr, "inet_aton_hname(\"%s\", ...) fallo'\n", str);
		exit(1);
	}
	ERRSYS( connect (sockfd, (struct sockaddr *)&s_in, sizeof(s_in)) );
	return sockfd;
}
int main(int argc, const char *argv[]) {
	int sockfd;
	fd_set rfds, wfds, efds;
	struct timeval tv; 
	char buf[1024];
	int nread;
	int nselect;

	if (argc != 3) {
		fprintf(stderr, "uso: %s HOST PORT\n", argv[0]);
		return 255;
	}
	sockfd=conecta_a (argv[1], atoi(argv[2]));
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
		ERRSYS( nselect=select (FD_SETSIZE, &rfds, &wfds, &efds, &tv) );

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
