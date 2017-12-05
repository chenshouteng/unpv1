
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>



//#include	"unp.h"
#define	SERV_PORT		 9877	
#define	LISTENQ		1024
#define	MAXLINE		4096


void str_echo(int sockfd)
{
	char buff[MAXLINE];
    int length=0;
	printf("server begin recv\n");
	while(length=recv(sockfd,buff,MAXLINE,0)) //这里是分包接收，每次接收4096个字节
    {
        if(length<0)
        {
            perror("recv");
            exit(-1);
        }
		printf("server send\n");
		if (send(sockfd,buff,MAXLINE,0) < 0)
		{
			perror("Send");
			exit(-1);
		}
		bzero(buff, sizeof(buff));
	}
}

int main(int argc, char **argv)
{

	int					listenfd, connfd,fpid;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
    //建立socket连接
    if ((listenfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("socket");
        exit(1);
    }
	printf("create socket success!\n");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

 // 设置套接字选项避免地址使用错误，为了允许地址重用，我设置整型参数（on）为 1 （不然，可以设为 0 来禁止地址重用）
    int on=1;  
    if((setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(-1);  
    }

	if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind");
        exit(-1);
	}
	printf("Bind success!\n");
	if(listen(listenfd, LISTENQ) == -1)
	{
		perror("listen");
        exit(-1);
	}

	for ( ; ; ) 
	{
		clilen = sizeof(cliaddr);
		printf("begin accept!\n");
		if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
		{
			perror("accept");
			exit(-1);			
		}
		printf("begin fork!\n");
		fpid=fork();   
		if (fpid < 0)   
		{
			perror("fork");
			exit(-1);
		} 
		else if (fpid == 0) //child process
		{  
			close(listenfd);	// close listening socket
			str_echo(connfd);	// process the request
			exit(0);
        }
		close(connfd);			// parent closes connected socket
	}
}
