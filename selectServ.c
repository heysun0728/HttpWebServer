/* include fig01 */
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
#define MAXLINE 8096
int
main(int argc, char **argv)
{
	int				i, maxi, maxfd, listenfd, connfd, sockfd,port=-1;
	int				nready, client[FD_SETSIZE];
	ssize_t				n;
	fd_set				rset, allset;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
	struct timeval timeout;
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


	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port);

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	maxfd = listenfd;			/* initialize */
	maxi = -1;				/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */

	FD_ZERO(&allset);		
	FD_SET(listenfd, &allset);	

	
	for ( ; ; ) {

		rset = allset;		/* structure assignment */
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s, port %d\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
					ntohs(cliaddr.sin_port));
#endif

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd;	/* save descriptor */
					break;
				}
			//if (i == FD_SETSIZE)
			//	err_quit("too many clients");

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		//printf("maxi=%d\n",maxi);
		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				respond(sockfd,path);
				/*if ( (n = read(sockfd, buf, MAXLINE)) == 0) {
						//4connection closed by client
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} else
					printf("%s\n",buf);
					//Writen(sockfd, buf, n);
				*/
				close(sockfd);
				FD_CLR(sockfd, &allset);
				client[i] = -1;
				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
/* end fig02 */
