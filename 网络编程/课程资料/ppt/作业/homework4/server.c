#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 9999
#define BACKLOG 5
#define MAXDATASIZE 1000


typedef struct CLIENT{
	int fd;
	char *name;
	struct sockaddr_in addr;
	char *data;
}CLIENT;

void process_cli(struct CLIENT*client, char *recvbuf, int len);
void savedata(char *recvbuf, int len, char *data);

int main(void) 
{
	int i, maxi, maxfd, sockfd;
	int nready;
	ssize_t n;
	fd_set rset, allset;
	int listenfd, connectfd;
	struct sockaddr_in server;
	struct CLIENT client[FD_SETSIZE];
	char recvbuf[MAXDATASIZE];
	int sin_size;
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("Create socket failed.");
		exit(-1);
	}
	int opt = SO_REUSEADDR;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt,
	sizeof(opt));
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr))==-1) {
		perror("Bind error.");
		exit(-1);
	}
	if (listen(listenfd, BACKLOG)== -1) {
		perror("listen error.");
		exit(-1);
	}
	sin_size= sizeof(struct sockaddr_in);
	maxfd = listenfd;
	maxi = -1;
	for (i =0; i< FD_SETSIZE;i++)
		client[i].fd = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd,&allset);
	while (1) 
	{
		struct sockaddr_in addr;
		rset = allset;
		nready = select (maxfd+1, &rset, NULL,NULL,NULL);
		if (FD_ISSET(listenfd,&rset)) 
		{
			if ((connectfd = accept(listenfd, (struct sockaddr *)&addr,&sin_size)) == -1) {
				perror("accept error.");
				continue;
			}
			for (i = 0; i < FD_SETSIZE;i++) 
			{
				if (client[i].fd< 0) 
				{
					client[i].fd = connectfd;
					client[i].name =
					malloc(sizeof(char)*MAXDATASIZE);
					client[i].addr = addr;
					client[i].data =
					malloc(sizeof(char)*MAXDATASIZE);
					client[i].name[0] = '\0';
					client[i].data[0] = '\0';
					printf(" You got a connect from %s:%d.\n ",
					inet_ntoa(client[i].addr.sin_addr), client[i].addr.sin_port);
					send(connectfd,
					"Welcome\n", 22, 0);
				break;
				} 
		    }
			if (i == FD_SETSIZE) 
				printf("too many cllients.\n");
			FD_SET(connectfd,&allset);
			if (connectfd > maxfd) 
			    maxfd = connectfd;
			if (i > maxi) 
				maxi =i;
			if (--nready <=0 ) 
				continue;
		}
		for (i = 0; i <= maxi; i++) 
		{
			if ((sockfd = client[i].fd) < 0) 
				continue;
			if (FD_ISSET(sockfd,&rset)) 
			{
				if (( n = recv(sockfd, recvbuf, MAXDATASIZE,0)) ==0) 
				{
					close(sockfd);
					printf("Client (%s) closed connection. User ’s data: %s\n", client[i].name, client[i].data);
					FD_CLR(sockfd,&allset);
					client[i].fd = -1;
					free(client[i].name);
					free(client[i].data);
				} 
				else
				    process_cli(&client[i], recvbuf, n);
				if ( --nready <=0 ) 
					break;
			} 
	    } 
    }
	close(listenfd);
}

void process_cli(CLIENT*client, char* recvbuf, int len)
{
	int i1;
	char sendbuf[MAXDATASIZE];
	recvbuf[len] ='\0';
	if(strlen(client->name) == 0) 
	{
		memcpy(client->name,recvbuf, len);
		printf("Client's name is %s.\n",client->name);
		return;
	}
	printf("Receivedclient( %s )message:%s\n",client->name, recvbuf);
	savedata(recvbuf,len,client->data);
	for (i1 = 0; i1< len - 1; i1++) 
	{
		sendbuf[i1] =recvbuf[len - i1 -2];
	}
	sendbuf[len - 1] ='\0';
	send(client->fd,sendbuf,strlen(sendbuf),0);
}

void savedata(char *recvbuf, int len, char *data)
{
	int start =strlen(data);
	int i;
	for (i = 0; i <len; i++) 
	{
		data[start + i]= recvbuf[i];
	} 
}
