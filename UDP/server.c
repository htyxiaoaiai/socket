#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
	{
		perror("sock");
		return 1;
	}
	
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(8080);
//	local.sin_addr.s_addr = inet_addr("127.0.0.1");
	local.sin_addr.s_addr = htons(INADDR_ANY);
	//bind
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		perror("bind");
		close(sock);
		return 2;
	}
	else
	{
		printf("bind success...\n");
	}

	struct sockaddr_in peer;
	socklen_t len =sizeof(struct sockaddr_in);
	char buf [1024];
	while(1)
	{
		ssize_t _s = recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&peer,&len);
		if(_s > 0)
		{
			buf[_s -1] = '\0';
			printf("client # %s",buf);
		}
		else if(_s == 0)
		{
			printf("client is closed...\n");
			break;
		}
		else
		{
			printf("read error\n");
			break;
		}

		if(sendto(sock,buf,sizeof(buf),0,(struct sockaddr*)&peer,len) < 0)
		{
			perror("sendto");
			return 4;
		}
		printf("sendto succes...\n");
	}
	close(sock);

	return 0;
}
