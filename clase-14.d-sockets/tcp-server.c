#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define PORT 12348

#define tostr(x) #x
#define ERRSYS(call) do { if ( (call) < 0) { perror(tostr(call));exit(1); }} while (0)

void servicio(int fd) {
	const char *array[] = { "TRES", "DOS", "UNO", "b00M!", NULL };
	const char **strp=array;
	char buf[1024];
	char *str=buf;
	FILE *f;

	if ( (f=fdopen(fd, "r+")) == NULL ) {
		perror("fdopen()");
		exit(1);
	}
	while(fgets(str, sizeof(buf)-(str-buf), f)) {
		if (str[0]=='\r' || str[0]=='\n') break;
		str+=strlen(str);
	}
	fprintf(f,"HTTP/1.0 200 Ok macho...\n\n");
	fputs(buf,f);
	for(;*strp;strp++) {
		fprintf(f, *strp);
		sleep(1);
	}
	fclose(f);
	close(fd);
}
int main(void)
{
	int sockfd, servfd;
	struct sockaddr_in addr_in;
	int opt;
	/*
	if ((sockfd=socket(PF_INET, SOCK_STREAM, 0))<0) {
		perror("socket"); return 1;
		}
		*/
	ERRSYS(sockfd=socket(PF_INET, SOCK_STREAM, 0));
	memset(&addr_in, 0, sizeof(struct sockaddr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(PORT);
	addr_in.sin_addr.s_addr = INADDR_ANY;
	opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));
	ERRSYS (bind (sockfd, (struct sockaddr *)&addr_in, 
				sizeof(struct sockaddr_in)) );
	ERRSYS (listen(sockfd, 20));
	while ((servfd=accept(sockfd, NULL, 0))>0) {
		switch(fork()) {
			case 0: /* hijo */
				close(sockfd);
				servicio(servfd);
				break;
			case -1: /* error */
				perror("fork()");
				break;
			default: /* padre */
				close(servfd);
		}
	}
	return 0;
}
