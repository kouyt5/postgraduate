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
#define BACKLOG 2 
#define MAXDATASIZE 100 


void process_cli(int connectfd,struct sockaddr_in client); 
void *start_routine(void *arg); 

struct ARG { 
int connfd; 
struct sockaddr_in client; 
}; 

int main(void) 
{ 
	int listenfd, connectfd; 
	pthread_t tid; 
	struct ARG *arg; 
	struct sockaddr_in server, client; 
	int sin_size; 
	if((listenfd=socket(AF_INET, SOCK_STREAM, 0))==-1) 
	{ 
		perror("Create socket failed."); 
		exit(-1); 
	} 
	int opt = SO_REUSEADDR; 
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 
	bzero(&server, sizeof(server)); 
	server.sin_family = AF_INET; 
	server.sin_port = htons(PORT); 
	server.sin_addr.s_addr = htonl(INADDR_ANY); 
	if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr))==-1) { 
		perror("Bind error."); 
		exit(-1); 
	} 
	if (listen(listenfd, BACKLOG) == -1) { 
		perror("listen error."); 
		exit(-1); 
	} 
	sin_size = sizeof(struct sockaddr_in); 
	while(1) { 
		if ((connectfd = accept(listenfd, (struct sockaddr *)&client, &sin_size)) == -1) 
		{ 
			perror("accept error."); 
			exit(-1); 
		} 
		arg = (struct ARG*)malloc(sizeof(struct ARG)); 
		arg -> connfd = connectfd; 
		memcpy((void *)&arg -> client, &client, sizeof(client)); 
		if (pthread_create(&tid, NULL, start_routine, (void *) arg)) 
		{ 
			perror("Creat pthread error."); 
			exit(-1); 
		} 
	} 
	close(listenfd); 
} 

void process_cli(int connectfd, struct sockaddr_in client) 
{ 
	char recvbuf[MAXDATASIZE],sendbuf[MAXDATASIZE],client_name[MAXDATASIZE]; 
	int recvlen,i; 
	printf("You got a connection from %s.\n",inet_ntoa(client.sin_addr)); 
	send(connectfd,"Welcome to my server.\n",22,0); 
	recvlen=recv(connectfd,client_name,MAXDATASIZE,0); 
	if(recvlen==0) 
	{ 
		close(connectfd); 
		printf("client disconnectde.\n"); 
		return; 
	} 
	else if(recvlen<0) 
	{ 
		close(connectfd); 
		printf("client broked.\n"); 
		return; 
	} 
	client_name[recvlen]='\0'; 
	printf("Client name is %s.\n",client_name); 
	while(recvlen=recv(connectfd, recvbuf, MAXDATASIZE,0)) 
	{
		recvbuf[recvlen]='\0'; 
		printf("Received client< %s > message: %s",client_name,recvbuf); 
		for(i=0;i<recvlen;i++) 
		    sendbuf[i]=recvbuf[recvlen-i-1]; 
		sendbuf[recvlen]='\0'; 
		send(connectfd,sendbuf, strlen(sendbuf),0); 
	} 
	printf("Client:%s disconnected.\n",client_name); 
	close(connectfd); 
} 

void *start_routine(void *arg)
{ 
	struct ARG *info; 
	info = (struct ARG *)arg; 
	process_cli(info -> connfd, info -> client); 
	free(arg); 
	pthread_exit(NULL); 
}
