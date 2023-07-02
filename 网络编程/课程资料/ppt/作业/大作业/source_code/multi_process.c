#include <sys/socket.h>
#include <signal.h>
#include "request.h"
#include "multi_process.h"


void main_loop(int socket)
{
    while(1){
        /* take a call and buffer it */
        int fd = accept( socket, NULL, NULL );
        if(fd == -1)
            break;
	signal(SIGCHLD, SIG_IGN);
        pid_t fpid = fork();
        if(fpid < 0)
            break;
        else if(fpid == 0)
        {
	    printf("This is child process\n");
            close(socket);
            do_request(fd);
            close(fd);
            break;
        }
        else
	{
	    printf("******************************\n");
	    printf("Created child process %d\n", fpid);
            close(fd);
	}
    }
}
