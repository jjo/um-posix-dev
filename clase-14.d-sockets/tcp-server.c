#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define tostr(x) #x
#define ERRSYS(call) do { if ( (call) < 0) { perror(tostr(call));exit(1); }} while (0)

void servicio(int fd) {
	const char *array[] = { "TRES\n", "DOS\n", "UNO\n", "b00M!\n", NULL };
	const char **strp=array;
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
	fprintf(f,"STR=%s\n", buf);
	for(;*strp;strp++) {
		fputs(*strp, f);
		usleep(400000);
	}
	fclose(f);
	close(fd);
}
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
	ERRSYS(sockfd=socket(PF_INET, SOCK_STREAM, 0));
	
	/* 
	 * preparo el addr:port para donde esperar conns 
	 */
	memset(&addr_in, 0, sizeof(struct sockaddr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
	addr_in.sin_addr.s_addr = INADDR_ANY;

	/* 
	 * Por si quedo' semi-cerrado recie'n: fuerzo reuso
	 */
	opt = 1;
	ERRSYS(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)));

	/* 
	 * bind(): me ligo a un address:port 
	 */
	ERRSYS (bind (sockfd, (struct sockaddr *)&addr_in, 
				sizeof(struct sockaddr_in)) );
	/* 
	 * listen(): Permito hasta 20 conexiones pendientes
	 */
	ERRSYS (listen(sockfd, 20));

	/* 
	 * no me interesa el exit_value de mis hijos (evito
	 * zombies)
	 */
	
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
