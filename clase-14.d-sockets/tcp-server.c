#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void servicio(int fd);

int main(int argc, const char *argv[])
{
	int sockfd, servfd;
	struct sockaddr_in addr_in;
	int opt;
	unsigned short port;

	if (argc!=2) {
		fprintf(stderr, "uso: %s <port>\n", argv[0]);
		return 1;
	}
	port = atoi(argv[1]);
	if (port <= 0) {
		fprintf(stderr, "port=%s invalido\n", argv[1]);
		return 2;
	}

	/* 
	 * socket(): Creo el socket ... 
	 */
	if(( sockfd=socket(PF_INET, SOCK_STREAM, 0))<0) {
		perror("socket()");return 1;
	}
	
	/* 
	 * preparo el addr:port para donde esperar conns 
	 */
	memset(&addr_in, 0, sizeof(struct sockaddr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_in.sin_port = htons(port);

	/* 
	 * Por si quedo' semi-cerrado recie'n: fuerzo reuso
	 */
	opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt))<0) {
		perror("setsockopt()");return 1;
	}

	/* 
	 * bind(): me ligo a un address:port 
	 */
	if (bind (sockfd, (struct sockaddr *)&addr_in, 
				sizeof(struct sockaddr_in))<0) {
		perror("bind()");return 1;
	}
	/* 
	 * listen(): Permito hasta 20 conexiones pendientes
	 */
	if ((listen(sockfd, 20))<0) {
		perror("listen()");return 1;
	}

	/* no me interesa el exit_value de mis hijos (evito zombies) */
	signal(SIGCHLD, SIG_IGN);

	/* 
	 * accept(): duermo hasta que entre una conexion 
	 */
	while ((servfd=accept(sockfd, NULL, 0))>0) {
		switch(fork()) {
			case 0: 
				/* hijo */
				close(sockfd);
				servicio(servfd);
				_exit(0);
				break;
				
			case -1: /* error */
				perror("fork()");
				break;

			default: 
				/* padre */
				close(servfd);
		}
	}
	return 0;
}

void servicio(int fd) {
	const char *array[] = { "TRES\n", "DOS\n", "UNO\n", "b00M!\n", NULL };
	const char **elemp=array;
	char buf[1024];
	char *str=buf;
	FILE *f;

	if ( (f=fdopen(fd, "r+")) == NULL ) {
		perror("fdopen()");
		exit(1);
	}
	fprintf(f,"Hola, aqui %s ...\ntipea y termina con 2 <Enters>\n",
			getlogin());
	fflush(f);
	while(fgets(str, sizeof(buf)-(str-buf), f)) {
		if (str[0]=='\r' || str[0]=='\n') break;
		str+=strlen(str);
	}
	setbuf(f, NULL);
	/* muestro string */
	fprintf(f,"STR=%s\n", buf);
	/* cuenta regresiva :) */
	for(;*elemp;elemp++) {
		fputs(*elemp, f);
		usleep(400000);
	}
	fclose(f);
	close(fd);
}
