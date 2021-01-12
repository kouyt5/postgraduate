#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 	1234
#define BACKLOG	1
#define MAXSIZE 1000


void reverse(char str[], int len)
{
	int beg,end;
	for(beg = 0, end = len-1; beg < end; beg++ , end--)
	{
 		char temp = str[beg];
		str[beg] = str[end];
		str[end] = temp;
	}
}

int main(void)
{	
	int 			listenfd, connectfd;
	struct sockaddr_in	server, client;	
	int			sin_size;
	char 			str[MAXSIZE];
	int 			num;
	int 			opt = SO_REUSEADDR;
	
	if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Create socket failed.");	
		exit(-1);	
	}

	
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	bzero(&server, sizeof(server));

	server.sin_family = AF_INET;	
	server.sin_port = htons(PORT);	
	server.sin_addr.s_addr = htonl(INADDR_ANY);	

	if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) 		
	{	
		perror("Bind error.");
		exit(-1);	
	}	

	if (listen(listenfd, BACKLOG) == -1) 
	{		
		perror("listen error.");
		exit(-1);
	}

	sin_size = sizeof(struct sockaddr_in);		
	while(1)
 	{	
		if ((connectfd = accept(listenfd, (struct sockaddr *)&client, &sin_size)) == -1)
		{	
			perror("accept error.");	
			exit(-1);	
		}	
		printf("The connection form %s:%d\n", inet_ntoa(client.sin_addr), client.sin_port);	
		send(connectfd, "HELLO!", 22, 0);		
		
		while(1)
		{
			memset(str, '\0', strlen(str));
			if( (num = recv(connectfd, str, MAXSIZE, 0)) == -1) 
			{
				perror("recv error.");		
				exit(1);
			}
			reverse(str,num);
			send(connectfd, str, num, 0);
		}
		close(connectfd);
	} 

	close(listenfd);
}

