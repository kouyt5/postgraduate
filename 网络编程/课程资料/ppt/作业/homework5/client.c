#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
#include <time.h>
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
    struct hostent *he;         
    struct sockaddr_in server; 
    int i=1;
    int o=1;
    int e = 1;
    if (argc != 2)         
     {                  
      printf("Usage: %s  <IP address>\n", argv[0]);  
      exit(-1);        
     }
    if ((he = gethostbyname(argv[1])) == NULL) 
    {
      perror("gethostbyname error.");        
      exit(1);    
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
  while(o)
  { 
   if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) 
   {                   
      printf("重启%d次.\n",e); 
        e++;
	int l;
      for(l=0;l<1000000000;l++)
         {}
        continue;
   }
    o=0;
 }
   if(((numbytes = recv(fd, buf, MAXDATASIZE, 0)) == -1))  
   {           
      perror("recv error.");        
      exit(1);    
    }

    buf[numbytes] = '\0';     
    printf("收到服务器消息: %s",buf);
    printf("你的名字:");
    scanf("%s",buf);      
   if( ((numbytes = send(fd, buf, strlen(buf), 0)) == -1))
    {     
      perror("send error.");
      exit(1);    
     }  
   while(i)      
    {   
      printf("任意输入字符串:");      
      scanf("%s",buf);
     if(strlen(buf)<1)
      i=0;
     if( ((numbytes = send(fd, buf, strlen(buf), 0)) == -1)) 
      {     
        perror("send error."); 
        exit(1);
      } 
     if( ((numbytes = recv(fd, buf, MAXDATASIZE, 0)) == -1))
      {
        perror("read error.");
        exit(1);
      }
      buf[numbytes] = '\0';  
      printf("服务端信息:%s\n",buf);   
      printf("\n");  
    }
 close(fd);
}
