#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

void catch(int sig)
{
	int status = 0;
	while(waitpid(-1,&status,WNOHANG) > 0)
	{
		printf("catch a signal %d ppid:%d\n",sig,getppid());
		printf("exit code %d\n",(status>>8)&0xff);
		printf("exit signal %d\n",status&0xff);
	}
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
	//local.sin_addr.s_addr = inet_addr("127.0.0.1");
	local.sin_addr.s_addr = htonl(INADDR_ANY);
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
		
		pid_t id = fork();
		if(id < 0)
		{
			perror("fork");
			close(sock);
			return 4;
		}
		else if(id == 0)
		{
			char buf[1024];
		while(1)
		{

			printf("get connection ,ip is :%s port is :%d\n",inet_ntoa(client_sock.sin_addr.s_addr),client_sock.sin_port);
			ssize_t _s = read(new_fd,buf,sizeof(buf));
			if(_s > 0)
			{
				buf[_s-1] ='\0';
			}
			else if(_s == 0)
			{
				close(new_fd);
			
				break;
			}
			else
			{
				break;
			}	
			printf("client :%s\n",buf);
			write(new_fd,buf,strlen(buf));
		}
		}
		else
		{
			signal(SIGCHLD,catch);
		}
	}
	return 0;
}
