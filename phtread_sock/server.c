#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

void * thread_run(void * arg)
{
	char buf[1024];
	int fd = (int)arg;
	printf("create a new thread...\n");
		while(1)
		{
			ssize_t _s = read(fd,buf,sizeof(buf));
			if(_s > 0)
			{
				buf[_s-1] ='\0';
			}
			else if(_s == 0)
			{
				printf("client close...\n");
				//close(fd);
				break;
			}
			else
			{
				break;
			}	
			printf("client :%s\n",buf);
			write(fd,buf,strlen(buf));
		}
		close(fd);
		pthread_exit(0);
}

int main()
{

	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("scok");
		return -1;
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(8080);
	local.sin_addr.s_addr = inet_addr("192.168.234.129");
	//local.sin_addr.s_addr = htonl(INADDR_ANY);
	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	//bind
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		perror("bind");
		close(sock);
		return 2;
	}

	//listen
	if(listen(sock,5)< 0)
	{
		perror("listen");
		close(sock);
		return 3;
	}

	printf("bind and listen succes,wait accept..\n");

	struct sockaddr_in client_sock;
	socklen_t len = sizeof(client_sock);
	while(1)
	{
		int new_fd  = accept(sock,(struct sockaddr*)&client_sock,&len);
		if(new_fd < 0)
		{
			perror("accept");
			close(sock);
			return 3;
		}

		printf("get connection ,ip is :%s port is :%d\n",inet_ntoa(client_sock.sin_addr.s_addr),client_sock.sin_port);
		//create pthread
		pthread_t id; 
		if(pthread_create(&id,NULL,thread_run,(void*)new_fd) < 0)
		{
			perror("phthread_create");
			close(sock);
			return 4;
		}

		//线程分离
		pthread_detach(id);
	}
	close(sock);
	return 0;
}
