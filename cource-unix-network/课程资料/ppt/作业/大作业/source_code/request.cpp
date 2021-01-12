//Request.cpp来实现服务器的各种响应
#include "request.h"
/* ------------------------------------------------------ *
   read_til_crnl(FILE *)
   skip over all request info until a CRNL is seen
   ------------------------------------------------------ */
void read_til_crnl(FILE *fp)
{
    char        buf[BUFSIZ];
    while( fgets(buf,BUFSIZ,fp) != NULL && strcmp(buf,"\r\n") != 0 );
}
/* ------------------------------------------------------ *
   process_rq( char *rq, int fd )
   分析客户端的请求，分析然后做出相应的应答
   rq is HTTP command:  GET /foo/bar.html HTTP/1.0
   ------------------------------------------------------ */
pid_t process_rq( char *rq, int fd )
{
    char        cmd[BUFSIZ], arg[BUFSIZ];
    pid_t fpid;
    /* create a new process and return if not the child */
    if ((fpid = fork()) != 0 )
        return fpid;
    strcpy(arg, "./");                /* precede args with ./ */
    if ( sscanf(rq, "%s%s", cmd, arg+2) != 2 )
        return fpid;
    if ( strcmp(cmd,"GET") != 0 )
        cannot_do(fd);
    else if ( not_exist( arg ) )
        do_404(arg, fd );
    else if ( isadir( arg ) )
        do_ls( arg, fd );
    else
        do_cat( arg, fd );
    return fpid;
}
/* ------------------------------------------------------ *
   the reply header thing: all functions need one
   if content_type is NULL then don't send content type
   ------------------------------------------------------ */
void header( FILE *fp, char *content_type )
{
    fprintf(fp, "HTTP/1.0 200 OK\r\n");
    if ( content_type )
        fprintf(fp, "Content-type: %s\r\n", content_type );
}
/* ------------------------------------------------------ *
   simple functions first:
        cannot_do(fd)       unimplemented HTTP command
    and do_404(item,fd)     no such object
   ------------------------------------------------------ */
void cannot_do(int fd)
{
    FILE        *fp = fdopen(fd,"w");
    fprintf(fp, "HTTP/1.0 501 Not Implemented\r\n");
    fprintf(fp, "Content-type: text/plain\r\n");
    fprintf(fp, "\r\n");
    fprintf(fp, "That command is not yet implemented\r\n");
    fclose(fp);
    exit(0);
}
void do_404(char *item, int fd)
{
    FILE        *fp = fdopen(fd,"w");
    fprintf(fp, "HTTP/1.0 404 Not Found\r\n");
    fprintf(fp, "Content-type: text/plain\r\n");
    fprintf(fp, "\r\n");
    fprintf(fp, "The item you requested: %s\r\nis not found\r\n",
            item);
    fclose(fp);
    exit(0);
}
/* ------------------------------------------------------ *
   the directory listing section
   isadir() uses stat, not_exist() uses stat
   do_ls runs ls. It should not
   ------------------------------------------------------ */
int isadir(char *f)
{
    struct stat info;
    return ( stat(f, &info) != -1 && S_ISDIR(info.st_mode) );
}
int not_exist(char *f)
{
    struct stat info;
    return( stat(f,&info) == -1 );
}

void do_ls(char *dir, int fd)
{
    FILE        *fp ;

    fp = fdopen(fd,"w");
    header(fp, "text/plain");
    fprintf(fp,"\r\n");
    fflush(fp);
    dup2(fd,1);
    dup2(fd,2);
    close(fd);
    
    execlp("ls","ls","-l",dir,NULL);
    perror(dir);
    exit(0);
}
char * file_type(char *f)
/* returns 'extension' of file */
{
    char        *cp;
    if ( (cp = strrchr(f, '.' )) != NULL )
        return cp+1;
    return "";
}
/* ------------------------------------------------------ *
   do_cat(filename,fd)
   sends back contents after a header
   ------------------------------------------------------ */

void do_cat(char *f, int fd)
{
    char        *extension = file_type(f);
    char        *content = "text/plain";
    FILE        *fpsock, *fpfile;
    int        c;

    if ( strcmp(extension,"html") == 0 )
        content = "text/html";
    else if ( strcmp(extension, "gif") == 0 )
        content = "image/gif";
    else if ( strcmp(extension, "jpg") == 0 )
        content = "image/jpeg";
    else if ( strcmp(extension, "jpeg") == 0 )
        content = "image/jpeg";

    fpsock = fdopen(fd, "w");
    fpfile = fopen( f , "r");
    if ( fpsock != NULL && fpfile != NULL )
    {
        header( fpsock, content );
        fprintf(fpsock, "\r\n");
        while( (c = getc(fpfile) ) != EOF )
            putc(c, fpsock);
        fclose(fpfile);
        fclose(fpsock);
    }
    exit(0);
}

void do_request(int fd)
{
    char request[BUFSIZ];
    pid_t fpid;
    int status;
    FILE* fpin;

    if((fpin = fdopen(fd, "r" )) == NULL){
	printf("Error while open fd : %d\n", fd, errno);
	return;
    }
    /* read request */
    fgets(request,BUFSIZ,fpin);
    printf("got a call: request = %s", request);
    printf("******************************\n");
    read_til_crnl(fpin);

    /* do what client asks */
    fpid = process_rq(request, fd);
    fclose(fpin);
    waitpid(fpid, &status, 0);
}
