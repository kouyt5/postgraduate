#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <errno.h>


void read_til_crnl(FILE *fp);
pid_t process_rq( char *rq, int fd );
void header( FILE *fp, char *content_type );
void cannot_do(int fd);
void do_404(char *item, int fd);
int isadir(char *f);
int not_exist(char *f);
void do_ls(char *dir, int fd);
char * file_type(char *f);
void do_cat(char *f, int fd);
void do_request(int fd);
