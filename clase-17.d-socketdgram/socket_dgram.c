#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define tostr(x) #x
#define ERRSYS(call) do { if ( (call) < 0) { perror(tostr(call));exit(1); }} while (0)
#define PORT 6969
#define GROUP "239.1.2.3"	/* desde 224.0.0.0 hasta 239.255.255.255 */
int main(void)
{
     int sockfd;
     struct sockaddr_in addr_r, addr_w;
     int addrlen_r, addrlen_w;
     struct ip_mreq imr;
     char buf[1024];
     int nbuf;
     fd_set rfds;
     int cnt;
     int nselect;
     int opt;

     ERRSYS(sockfd = socket(PF_INET, SOCK_DGRAM, 0));

     memset(&addr_r, 0, sizeof(addr_r));
     addr_r.sin_family = AF_INET;
     addr_r.sin_port = htons(PORT);
     addr_r.sin_addr.s_addr = htonl(INADDR_ANY);

     memset(&addr_w, 0, sizeof(addr_w));
     addr_w.sin_family = AF_INET;
     addr_w.sin_port = htons(PORT);
     addr_w.sin_addr.s_addr = inet_addr(GROUP);


     imr.imr_multiaddr.s_addr = inet_addr(GROUP);
     imr.imr_interface.s_addr = htonl(INADDR_ANY);

     ERRSYS(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
			     (void *) &imr, sizeof(struct ip_mreq)));
     opt = 1;
     ERRSYS(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)));
     ERRSYS(bind(sockfd, (struct sockaddr *)&addr_r, sizeof(addr_r)));
     addrlen_r = sizeof(addr_r);
     addrlen_w = sizeof(addr_w);
     while (1) {
	     FD_ZERO(&rfds);
	     FD_SET(STDIN_FILENO, &rfds);
	     FD_SET(sockfd,       &rfds);

	     ERRSYS( nselect=select(1024, &rfds, NULL, NULL, NULL) );
	     if (FD_ISSET(STDIN_FILENO, &rfds)) {
		     nbuf=snprintf(buf, sizeof(buf), "%s dijo: ", getlogin());
		     ERRSYS( cnt=read(STDIN_FILENO, buf+nbuf, sizeof(buf)-nbuf) );
		     if (cnt==0) { fprintf(stderr, "stdin: EOF\n"); break; }
		     if (buf[nbuf]=='\n') continue;
		     fprintf(stderr, "** stdin -> red\n");

		     ERRSYS( cnt=sendto(sockfd, 
				     buf, nbuf+cnt, 0, 
				     (struct sockaddr *)&addr_w, addrlen_w));
	     }
	     if (FD_ISSET(sockfd, &rfds)) {
		     ERRSYS( cnt = recvfrom(sockfd, buf, sizeof(buf), 0, 
				     (struct sockaddr *)&addr_r, &addrlen_r));
		     if (cnt==0) { fprintf(stderr, "red: EOF\n"); break; }
		     fprintf(stderr, "** red (%s:%d)-> stdout\n",
				     inet_ntoa(addr_r.sin_addr),
				     ntohs(addr_r.sin_port));
		     ERRSYS( write(STDOUT_FILENO, buf, cnt) );
	     }
		
     }
     return 0;
}
