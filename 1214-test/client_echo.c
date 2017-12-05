
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>

#define	MAXLINE		4096
#define	SERV_PORT		 9877	

void str_cli(FILE *fp, int sockfd)
{
    char    sendline[MAXLINE], recvline[MAXLINE];

    while(fgets(sendline, MAXLINE, fp) != NULL)
    {
		printf("sendline : %s\n",sendline);
		if (send(sockfd,sendline,strlen(sendline),0) < 0)
		{
			perror("Send");
			exit(-1);
		}
		if (recv(sockfd,recvline,MAXLINE,0) < 0 )
		{
			perror("recv");
            exit(-1);
		}
		printf("recvline : %s\n",recvline);
        fputs(recvline, stdout);
    }
}

int main(int argc, char **argv)
{
	int	sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
	{
		perror("usage: tcpcli <IPaddress>");
		exit(-1);
	}


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("sockfd");
		exit(-1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
	{
        perror("inet_pton");    
        exit(-1);		
	}

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
        perror("connect");
        exit(-1);		
	}

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
