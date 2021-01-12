#include <event.h>
#include <sys/socket.h>
#include "request.h"
#include "io_mulplexing.h"

struct event_base* base;
void on_read(int sock, short event, void* arg)  
{  
    struct event* ev = (struct event*)arg;
    do_request(sock);
    event_del(ev);
    free(ev);
    ev = NULL;
    close(sock);
}  
  
void on_accept(int sock, short event, void* arg)  
{  
    struct sockaddr_in cli_addr;  
    int newfd, sin_size;  
    struct event* read_ev = (struct event*)malloc(sizeof(struct event));    
    newfd = accept(sock, NULL, NULL);
    if(newfd == -1)
    	return;  
    event_set(read_ev, newfd, EV_READ|EV_PERSIST, on_read, read_ev);  
    event_base_set(base, read_ev);  
    event_add(read_ev, NULL);  
}  
 
  
void main_loop(int socket)  
{   
    struct event listen_ev;  
    base = event_base_new();  
    event_set(&listen_ev, socket, EV_READ|EV_PERSIST, on_accept, NULL);  
    event_base_set(base, &listen_ev);  
    event_add(&listen_ev, NULL);  
    event_base_dispatch(base);    
}
