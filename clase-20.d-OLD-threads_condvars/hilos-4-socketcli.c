/* $Id: hilos-4-socketcli.c,v 1.3 2002/10/03 15:12:04 jjo Exp $ */
/*
 * Objetivo: POSIX threads: cliente "telnet" MT
 * Implementacion:
 * 	Luego de la creación del socket, se lanzan 3 threads:
 * 	1) stdin->net     copia de uno a otro, reseteando un contador ...
 * 	2) net->stdout    ... a un valor > 0
 * 	3) ticks          decrementa el contador una vez por segundo
 * 	
 * 	El "main" espera (cond_wait) que dicho contador sea 0 y termina.
 * 
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define tostr(x) #x
#define ERRSYS(call) do { if ( (call) < 0) { perror(tostr(call));exit(1); }} while (0)
#define TIMEOUT_VAL 10
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
struct hilo_cond {
	pthread_mutex_t mutex;
	pthread_cond_t  cond;
	int val;
};
struct hilo_fildes {
	int fd_in;
	int fd_out;
	struct hilo_cond *cond;
};
int hilo_cond_set(struct hilo_cond *hc, int val)
{
	pthread_mutex_lock(&hc->mutex);
	hc->val = val;
	pthread_mutex_unlock(&hc->mutex);
	pthread_cond_signal(&hc->cond);
	return val;
}
int hilo_cond_delta(struct hilo_cond *hc, int delta)
{
	int val;
	pthread_mutex_lock(&hc->mutex);
	val = hc->val + delta;
	if (val<0) val=0;
	hc->val = val;
	/* fprintf(stderr, "DELTA val=(%d)\n", hc->val); */
	pthread_mutex_unlock(&hc->mutex);
	pthread_cond_signal(&hc->cond);
	
	return val;
}
int hilo_cond_waitfor(struct hilo_cond *hc, int val)
{
	fprintf(stderr, "ENTRO val=(%d,%d)\n", hc->val, val);
	pthread_mutex_lock(&hc->mutex);
	while (hc->val != val) 
		pthread_cond_wait(&hc->cond, &hc->mutex);
	pthread_mutex_unlock(&hc->mutex);
	fprintf(stderr, "SALIO val=(%d,%d)\n", hc->val, val);
	return val;
}
void * hilo_fildes_copy(void *data)
{
	struct hilo_fildes *hfd=data;
	int nread;
	char buf[1024];
	fprintf(stderr, "tid=%ld fd_int=%d fd_out=%d\n", 
			pthread_self(), hfd->fd_in, hfd->fd_out);
	while ((nread=read(hfd->fd_in, buf, sizeof(buf)))>0) {
		hilo_cond_set(hfd->cond, TIMEOUT_VAL);
		write(hfd->fd_out, buf, nread);
	}
	fprintf(stderr, "tid=%ld nread=%d\n", 
			pthread_self(), nread);
	hilo_cond_set(hfd->cond, 0);
	return NULL;
}
void *hilo_ticks(void *data)
{
	struct hilo_cond *hc= data;
	while(1) {
		hilo_cond_delta(hc, -1);
		fprintf(stderr, "%c\b", hc->val + '0');
		sleep(1);
	}
	return NULL;
}
int main(int argc, const char *argv[]) {
	int sockfd;
	pthread_t tid_stdin_net, tid_net_stdout, tid_ticks;
	struct hilo_cond cond;
	struct hilo_fildes fildes_stdin_net = { STDIN_FILENO, -1, &cond };
	struct hilo_fildes fildes_net_stdout= { -1, STDOUT_FILENO, &cond};

	if (argc != 3) {
		fprintf(stderr, "uso: %s IP.AD.DR.ES PORT\n", argv[0]);
		return 255;
	}
	sockfd=conecta_a (argv[1], atoi(argv[2]));
	pthread_mutex_init(&cond.mutex, NULL);
	pthread_cond_init(&cond.cond, NULL);
	cond.val= 0;

	fildes_stdin_net.fd_out = sockfd;
	fildes_net_stdout.fd_in = sockfd;
	pthread_create(&tid_stdin_net, NULL, 
			hilo_fildes_copy, &fildes_stdin_net);
	pthread_create(&tid_net_stdout, NULL, 
			hilo_fildes_copy, &fildes_net_stdout);
	pthread_create(&tid_ticks, NULL,
			hilo_ticks, &cond);
	hilo_cond_set(&cond, TIMEOUT_VAL);
	hilo_cond_waitfor(&cond, 0);
	return 0;
}
