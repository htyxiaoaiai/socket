#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

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

	struct sockaddr_in server_sock;
	server_sock.sin_family = AF_INET;
	server_sock.sin_port = htons(atoi(argv[2]));
	server_sock.sin_addr.s_addr = inet_addr(argv[1]);

	int ret = connect(sock,(struct sockaddr*)&server_sock,sizeof(server_sock));
	if(ret < 0)
	{
		perror("connect");
		close(sock);
		return 2;
	}
	printf("connect success...\n");
	char buf[1024];
	while(1)
		{
			printf("please enter :");
			fflush(stdout);
			ssize_t _s = read(0,buf,sizeof(buf));
			if(_s > 0)
			{
				buf[_s] ='\0';
			}
			else if(_s == 0)
			{
	
				close(sock);
				break;
			}
			else
			{
				break;
			}	
			write(sock,buf,strlen(buf));
			printf("please wait...\n");
			read(sock,buf,sizeof(buf));
			printf("server # :%s",buf);
		}
	return 0;
}
