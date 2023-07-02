#include <sys/socket.h>
#include "request.h"
#include "iteration.h"


void main_loop(int socket)
{
    while(1){
        /* take a call and buffer it */
        int fd = accept( socket, NULL, NULL );
        if(fd == -1)
            break;
        do_request(fd);
        close(fd);
    }
}
