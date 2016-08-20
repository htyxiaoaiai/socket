#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{
	if(argc !=3)
	{
		printf("Usage : server [IP] [PORT] \n");
		return 1;
	}

	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("scok");
		return -1;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);


	socklen_t len =sizeof(struct sockaddr_in);
	char buf [1024];
	while(1)
	{
		printf("please enter :");
		fflush(stdout);
		ssize_t _s = read(0,buf,sizeof(buf));
		if(_s > 0)
		{
			buf[_s]='\0';
		}
		if(sendto(sock,buf,sizeof(buf),0,(struct sockaddr*)&server,len) < 0)
		{
		printf("you should run here...\n");
			perror("sendto");
			return 4;
		}
		printf("sendto succes...\n");

		ssize_t s = recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&server,&len);
		if(s > 0)
		{
			buf[s -1] = '\0';
			printf("server # %s",buf);
		}
		else if(s == 0)
		{
			printf("server is closed...\n");
			break;
		}
		else
		{
			printf("read error\n");
			break;
		}

	}
	close(sock);

	return 0;
}
