#define BUFSIZE 8096

void respond(int fd,char* path)
{
	//printf("respond is called\n");    
	static char buffer[BUFSIZE+1];
	int j, file_fd, buflen, len;
	long i, ret;
	char* fstr,final_path[1024];

	// get request from chrome
	ret = read(fd,buffer,BUFSIZE);   
	//printf("%s\n",buffer);

	if (ret<=0) exit(3);
	if (strncmp(buffer,"GET",3)&&strncmp(buffer,"get",3)) exit(3);

	//add '\0' to each element in array
	for(i=4;i<BUFSIZE;i++) {
		if(buffer[i] == ' ') {
		    buffer[i] = 0;
		    break;
		}
	}

	//return index.html
	if (!strncmp(&buffer[0],"GET /\0",6)||!strncmp(&buffer[0],"get /\0",6) )
		strcpy(buffer,"GET /index.html\0");

	// open
	strcpy(final_path, path);
	strcat(final_path, &buffer[5]);
	//printf("file=%s\n",final_path);
	if((file_fd=open(final_path,O_RDONLY))==-1) 
		write(fd, "Failed to open file", 19);

	//return sucess message
	sprintf(buffer,"HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", fstr);
	write(fd,buffer,strlen(buffer));

	//write to client
	while ((ret=read(file_fd, buffer, BUFSIZE))>0) {
		write(fd,buffer,ret);
	}

}
