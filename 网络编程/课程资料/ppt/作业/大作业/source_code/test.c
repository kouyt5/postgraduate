#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXBUF 2048
#define MAXLINE 1024
#define BILLION 1E9

/* Persistent state for the robust I/O (Rio) package */
/* $begin rio_t */
#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* Descriptor for this internal buf */
    int rio_cnt;               /* Unread bytes in internal buf */
    char *rio_bufptr;          /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;
/* $end rio_t */

int main(int argc, char** argv) {
  int clientfd, port;
  char* host;
  char request_buf[MAXBUF], receive_buf[MAXBUF];
  int nchildren, nloops;    //进程数量及每个进程发送的请求数量
  rio_t rio;

  if (argc != 5) {
    fprintf(stderr, "usage: %s <host> <port> <#children> <#loops/child>\n",
            argv[0]);
    exit(0);
  }
  host = argv[1];
  port = atoi(argv[2]);
  nchildren = atoi(argv[3]);
  nloops = atoi(argv[4]);

  sprintf(request_buf, "HEAD / HTTP/1.1\r\n");
  sprintf(request_buf, "%sHOST: %s: %d\r\n\r\n", request_buf, host, port);

  int pipe[2];
  Pipe(pipe);

  int i, j, pid;
  double totaltime = 0;
  for (i = 0; i < nchildren; i++) {
    if ((pid = Fork()) == 0) {
      Close(pipe[0]);
      double child_total_time = 0;
      for (j = 0; j < nloops; j++) {
        struct timespec requestStart, requestEnd;
        clock_gettime(CLOCK_REALTIME, &requestStart);

        clientfd = Open_clientfd(host, port);
        Rio_readinitb(&rio, clientfd);
        Rio_writen(clientfd, request_buf, strlen(request_buf));
        Rio_readlineb(&rio, receive_buf, MAXLINE);
        Close(clientfd);

        clock_gettime(CLOCK_REALTIME, &requestEnd);
        double accum = (requestEnd.tv_sec - requestStart.tv_sec) +
                       (requestEnd.tv_nsec - requestStart.tv_nsec) / BILLION;
        child_total_time += accum;

        // printf("%s\n", receive_buf);
      }
      // printf("children %d id done\n", i);
      // printf( "the child_total_time is %lf\n", child_total_time );
      // printf( "the child_avg_time is %lf\n", child_total_time /
      // (double)nloops );
      Write(pipe[1], &child_total_time, sizeof(child_total_time));
      exit(0);
    }
  }
  while (wait(NULL) > 0)
    ;
  Close(pipe[1]);
  double temp_time;
  while (Read(pipe[0], &temp_time, sizeof(temp_time)) > 0) {
    totaltime += temp_time;
  }
  printf("总响应时间 :%lfs\n", totaltime);
  printf("平均响应时间 :%lfs\n",
         totaltime / ((double)nloops * (double)nchildren));

  if (errno != ECHILD) app_error("wait error!!");

  exit(0);
}
