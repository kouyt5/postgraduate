#include        <stdio.h>
#include        <unistd.h>
#include        <sys/types.h>
#include        <sys/socket.h>
#include        <netinet/in.h>
#include        <netdb.h>
#include        <string.h>
#include        <stdlib.h>

//#include "iteration.h"
//#include "multi_process.h"
//#include "multi_thread.h"
//#include "io_mulplexing.h"
#include "thread_pool.h"
#define   HOSTLEN  256
#define   BACKLOG  1

int make_server_socket(int portnum);
int make_server_socket_q(int portnum, int backlog);
int main(int argc, char *argv[])
{
    int         sock, fd;
    sock = make_server_socket(5001);
    if ( sock == -1 ) exit(2);
    main_loop(sock);
    return 0;
}
/* ------------------------------------------------------ *
   make_server_socket(int portnum)
   创建一个服务器套接字,并调用listen监听
   ------------------------------------------------------ */
int make_server_socket(int portnum)
{
    return make_server_socket_q(portnum, BACKLOG);
}
int make_server_socket_q(int portnum, int backlog)
{
    struct  sockaddr_in   saddr;   /* build our address here */
    struct        hostent                *hp;   /* this is part of our    */
    char        hostname[HOSTLEN];     		/* address                */
    int        sock_id;               /* the socket             */
    sock_id = socket(PF_INET, SOCK_STREAM, 0);  /* get a socket */
    if ( sock_id == -1 )
        return -1;

    bzero((void *)&saddr, sizeof(saddr));   /* clear out struct     */
    gethostname(hostname, HOSTLEN);         /* where am I ?         */
    hp = gethostbyname(hostname);           /* get info about host  */
    /* fill in host part    */
    bcopy( (void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
    saddr.sin_port = htons(portnum);        /* fill in socket port  */
    saddr.sin_family = AF_INET ;            /* fill in addr family  */
    if ( bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0 )
        return -1;
    /** arrange for incoming calls **/
    if ( listen(sock_id, backlog) != 0 )
        return -1;
    return sock_id;
}
