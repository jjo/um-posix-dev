#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "webito.h"
void child_handler(int signum);
int servicio(int fd)
{
	char line[2048];
	FILE *stream;
	struct http_parser ht;
	stream=fdopen(fd, "r");
	http_parser_init(&ht);
	while(fgets(line, sizeof line -1, stream)) {
		http_parser_process_line(&ht, line);
		if(line[0]=='\n'||line[0]=='\r') break;	
	}
	fclose(stream);
	close(fd);
	return 0;
}
int main(int argc, char **argv) {
	int sockfd, conn_sockfd;
	int i;				/* para setsockopt */
	pid_t hijo; 				/* pid del hijo */
	struct sockaddr_in addr_local;		/* donde voy a escuchar */
	socklen_t addr_local_len;		/* longitud de addr_local */
	unsigned short port;
	int ret;

	/* Verifico el nro de argumentos */
	if (argc != 2) {
		fprintf(stderr, "Uso: %s <puerto>\n",
				argv[0]);
		return 1;
	}
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0))<0) {
		perror("socket");
		return 1;		
	}

	/* Se desea reutilizar la direccion local */
	i=1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

	/* Inicializa y configura la estructura de socket del servidor */ 
	memset(&addr_local, 0, sizeof(addr_local));
	addr_local.sin_family = AF_INET;
	port = atoi(argv[1]);
	if (port <=0) {
		fprintf(stderr, "port=%d invalido\n", port);
		return 255;
	}
	addr_local.sin_port = htons(port);
		
	/* Liga el socket a una direccion */
	addr_local_len = sizeof(addr_local);
	if ( (bind(sockfd, (struct sockaddr *)&addr_local, 
		addr_local_len)) < 0 ) {
		perror("bind");
		return 2;		
	}
	
	/* Aguarda a q arriben conexiones */
	if ( (listen(sockfd, 5)) < 0 ) {
		perror("listen");
		return 3;
	}

	printf("\nSOCKET %d ESCUCHANDO ...\n", port);
	printf("\tPuerto: %d\n", ntohs(addr_local.sin_port));
	printf("\tDireccion IP: %s\n", inet_ntoa(addr_local.sin_addr));
	
	signal(SIGCHLD, child_handler);
	/* Ejecuta indefinidamente la aceptacion de conexiones */
	while ((conn_sockfd = accept(sockfd, (struct sockaddr *)&addr_local, 
		&addr_local_len))>=0) {
		
		switch(hijo=fork()) {
		case -1: /* Hubo un problema con la creacion del hijo */ 
			break;
		case 0: /* Codigo del hijo, futuros ZOMBIES ... */
			close(sockfd);
			ret=servicio(conn_sockfd);
			_exit(ret);
			break;
			
		default: /* Codigo del padre */			
			close(conn_sockfd);
			break;
		}
		printf("\nNueva conexion desde %s:%d (pid=%d)\n", 
				inet_ntoa(addr_local.sin_addr),
				ntohs(addr_local.sin_port),
				hijo);
	}
	close(sockfd);
	return 0;
}
void child_handler(int signum) {
	int status;
	pid_t pid;
	while ((pid=waitpid(0, &status, WNOHANG))>=0) {
		fprintf(stderr, "(pid=%d enterrado)\n", pid);
	}
}
