#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 4096

int main(int argc, char *argv[]) {
		
	int sockfd;
	struct sockaddr_in addr_remoto;
	socklen_t addr_remoto_len;
	char buffer[MAX_SIZE];
	int nread;
	unsigned short port;
	
	/* Verifico el nro de argumentos */
	if (argc != 3) {
		fprintf(stderr, "Uso: %s <nro ip> <puerto>\n",
				argv[0]);
		return 255;
	}

	/* Inicializo y establezco la estructura del cliente
	 * :1: familia (AF_INET)
	 * :2: IP
	 * :3: PORT
	 */
	memset(&addr_remoto, 0, sizeof(addr_remoto));
	
	/* :1: familia */
	addr_remoto.sin_family = AF_INET;
	
	/* :2: IP 
	 *     convierto la dir. IP de string a sin_addr
	 */
	if ( !(inet_aton(argv[1], &addr_remoto.sin_addr))) {
		perror("inet_aton");
		return 255;
	}
	/* 
	 * :3: PORT
	 *     convierto el port a numerico y luego la paso de 
	 *     representacion de CPU -> red 
	 */
	port = atoi(argv[2]);
	addr_remoto.sin_port = htons(port);
	addr_remoto_len = sizeof(addr_remoto);
	
	/* Creo el socket TCP */	
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0))<0) {
		perror("socket");
		return 3;
	}

	/* Conecta al peer remoto */
	if (connect(sockfd, (struct sockaddr *)&addr_remoto, addr_remoto_len)) {
		perror("connect");
		return 4;
	}
	fprintf(stderr, "Cliente conectado.\nPara finalizar: <enter><enter>\n");
	
	/* Escribe stdin en el socket, ignorando lo que me escriba el server */
	while( (nread=read(STDIN_FILENO, buffer, sizeof buffer)) > 0) {
		if (nread==1 && buffer[0] == '\n' ) break; /* si <enter> solamente ... */
		if (write(sockfd, buffer, nread)<0) {
			perror("write"); return 1;	
		}
	}

	/* no me interesa seguir escribiendo ...*/
	shutdown(sockfd, SHUT_WR);

	/* leo todo lo que haya escrito el peer y lo copio a stdout */
	while( (nread=read(sockfd, buffer, sizeof buffer)) > 0) {
		write(STDOUT_FILENO, buffer, nread);
	}
	
	close(sockfd);
	return EXIT_SUCCESS;
}

/*
 * $Log: tcp_ip_client.c,v $
 * Revision 1.3  2004/08/20 21:22:14  jjo
 * . retoques
 *
 * Revision 1.2  2003/08/22 20:43:47  jjo
 * . Renombrado struct_client-> addr_remoto
 * . Uso read en vez de fgets
 * . 3er argumento: port
 * . while(read()) luego while(write()) para mostrar output del server
 * . shutdown() para "se~alizar" el fin de mis writes
 *
 */
