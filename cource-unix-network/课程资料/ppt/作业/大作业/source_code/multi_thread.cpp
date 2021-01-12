#include <sys/socket.h>
#include <pthread.h>
#include "request.h"
#include "multi_thread.h"

void* thread(void* fd);

void main_loop(int socket)
{
    int *connfd;
    pthread_t t_id;
    while(1){
	connfd = (int*)malloc(sizeof(int));
        /* take a call and buffer it */
        *connfd = accept( socket, NULL, NULL );
        if(*connfd == -1)
            break;
        pthread_create(&t_id, NULL, thread, (void*)connfd);
	printf("******************************\n");
	printf("Created child thread %lu\n", t_id);
    }
}
void* thread(void* arg)
{
    int fd = *((int *)arg);
    pthread_detach(pthread_self());
    free(arg);
    do_request(fd);
    pthread_exit(0);
}
