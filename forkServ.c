#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "respond.c"
#define LISTENQ 10000 

int main(int argc, char **argv)
{
	int			listenfd, connfd, status, port=-1;
	pid_t			childpid;
	socklen_t		clilen;
	struct sockaddr_in	cliaddr, servaddr;
	char* path="web/";

	//resolve argv
	if(argc>1&&(argc%2)==0){
		printf("argument not correct\n");
		return 0;
	}
	for(int i=1;i<argc;i++){
		if(strcmp(argv[i],"-p")==0){
			sscanf(argv[i+1], "%d", &port);
		}else if(strcmp(argv[i],"-r")==0){
			path=argv[i+1];
		}
	}
	if(port==-1) port=80;

	//socket
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port);

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

		if ( (childpid = fork()) == 0) {
			close(listenfd);	
			respond(connfd,path);	
			exit(0);
		}
		close(connfd);	
		wait(&status);
	}
}
