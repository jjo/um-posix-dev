#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 4096

int main(int argc, char *argv[]) {
		
	int sd;
	struct sockaddr_in struct_client;
	socklen_t struct_client_len;
	char buffer[MAX_SIZE];
	
	/* Verifico el nro de argumentos */
	if (argc < 2) {
		puts("Modo de uso: ./tcp_ip_client <nro ip>");
		return 1;
	}

	/* Creo el socket */	
	if ((sd = socket(PF_INET, SOCK_STREAM, 0))<0) {
		perror("socket");
		return 2;
	}

	/* Inicializo y establezco la estructura del cliente */
	memset(&struct_client, 0, sizeof(struct_client));
	struct_client.sin_family = AF_INET;
	struct_client.sin_port = htons(2411);
	
	if ( !(inet_aton(argv[1], &struct_client.sin_addr))) {
		perror("inet_aton");
		return 3;
	}
	
	/* Conecta al socket */
	struct_client_len = sizeof(struct_client);
	if (connect(sd, (struct sockaddr *)&struct_client, struct_client_len)) {
		perror("connect");
		return 4;
	}
	printf("Cliente conectado. !!!\n");
	
	/* Escribe stdin en el socket */
	while(fgets(buffer, MAX_SIZE, stdin) != NULL) {
		if (write(sd, buffer, strlen(buffer))<0) {
			perror("write"); return 1;	
		}
	}
	
	close(sd);
	
	return 0;
}

