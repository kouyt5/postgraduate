#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#define PORT 9999 
#define MAXDATASIZE 100 

int main(int argc, char *argv[]) 
{ 
	int fd, numbytes; 
	char buf[MAXDATASIZE]; 
	struct hostent * he; 
	struct sockaddr_in server; 
	int i=1; 
	if (argc != 2) 
	{ 
		printf("Input error!\n"); 
		exit(-1); 
	} 
	if ((he = gethostbyname(argv[1])) == NULL) 
	{ 
		perror("gethostbyname error."); 
		exit(-1); 
	} 
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{ 
		perror("Create socket failed."); 
		exit(1); 
	} 
	bzero(&server, sizeof(server)); 
	server.sin_family = AF_INET; 
	server.sin_port = htons(PORT); 
	server.sin_addr = *((struct in_addr *) he->h_addr); 
	if (i=connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) 
	{ 
		perror("connect failed."); 
		exit(1); 
	} 
	if( ((numbytes = recv(fd, buf, MAXDATASIZE, 0)) == -1)) { 
		perror("recv error."); 
		exit(1); 
	} 
	buf[numbytes] = '\0'; 
	printf("Server: %s",buf); 
	printf("Please input the name:"); 
	scanf("%s",buf); 
	if( ((numbytes = send(fd, buf, strlen(buf), 0)) == -1)) { 
		perror("send error."); 
		exit(1); 
	} 
	while(1) 
	{ 
		printf("Please input(max char:%d):",MAXDATASIZE); 
		scanf("%s",buf); 
		if(strlen(buf)<1) 
		    i=0; 
		if( ((numbytes = send(fd, buf, strlen(buf), 0)) == -1)) { 
			perror("send error."); 
			exit(1); 
		} 
		if( ((numbytes = recv(fd, buf, MAXDATASIZE, 0)) == -1)) { 
			perror("read error."); 
			exit(1); 
		} 
		buf[numbytes] = '\0'; 
		printf("Server:%s\n",buf); 
		printf("\n"); 
	} 
	close(fd); 
}

