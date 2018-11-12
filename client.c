#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/errno.h>
#include <string.h>
#define BUF_SIZE 4096

int main(int argc,char **argv){
	
	//check argv
	if(argc!=2){
		printf("argument error,please enter again\n");
		exit(3);
	}

	char request[BUF_SIZE],response[BUF_SIZE];
	char *targetHost=argv[1];
	char *header = "GET / HTTP/1.1\r\nHost: %s\r\n\r\n";
	int listenfd;
	struct addrinfo hints,*result;

	//get request
	snprintf(request, BUF_SIZE, header, targetHost);
	//printf("%s",request);

	//getaddrinfo
	memset(&hints, 0, sizeof(struct addrinfo));//clear hints
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_flags = AI_NUMERICSERV;
	getaddrinfo(targetHost, "80", &hints, &result);

	//socket
	listenfd = socket(result->ai_family, result->ai_socktype, 0);
	connect(listenfd, result->ai_addr, result->ai_addrlen);

	//request & respond
	printf("----------Request:----------\n%s\n", request);
	send(listenfd, request, strlen(request), 0); //post request
	printf("----------Response:----------\n");
	while(1){
		recv(listenfd, response, BUF_SIZE, 0);
		printf("%s", response);
		if(strlen(response)<BUF_SIZE) break;
	}
	shutdown(listenfd, SHUT_WR);

	return 0;	
}
