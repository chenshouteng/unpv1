#include<stdio.h>
#include<netinet/in.h>  
#include<sys/socket.h> 
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<time.h>
#include<pthread.h>

#define SIZE 1024

#define	SERV_PORT		 9878
char name[32];

void* pthread_recv(void * arg)
{
	char buffer[SIZE];
	int sockfd = *(int *)arg;
	while(1)
	{
		//用于接收信息
		memset(buffer,0,SIZE);
		if(sockfd > 0)
		{
			if((recv(sockfd,buffer,SIZE,0)) <= 0)
			{
				close(sockfd);
				exit(1);
			}
			printf("%s\n",buffer);
		}
	}

}
void* pthread_send(void * arg)
{
	//时间函数
	char buffer[SIZE],buf[SIZE];
	int sockfd = *(int *)arg;
	struct tm *p_curtime;
	time_t timep;

	while(1)
	{
		memset(buf,0,SIZE);
		fgets(buf,SIZE,stdin);//获取用户输入的信息
		memset(buffer,0,SIZE);
		time(&timep);
		p_curtime = localtime(&timep);
		strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", p_curtime);
		/*输出时间和客户端的名字*/
		strcat(buffer," \n\t昵称 ->");
		strcat(buffer,name);
		strcat(buffer,":\n\t\t  ");

		/*对客户端程序进行管理*/
		if(strncmp("e",buf,1)==0)
		{
			printf("该客户端下线...\n");
			strcat(buffer,"退出聊天室！");
			if((send(sockfd,buffer,SIZE,0)) <= 0)
			{
				perror("error send");
			}
			close(sockfd);
			sockfd = -1;
			exit(0);
		}
		else
		{
			
			strncat(buffer,buf,strlen(buf)-1);
			strcat(buffer,"\n");     		
			if((send(sockfd,buffer,SIZE,0)) <= 0)
			{
				 perror("send");
			}
		}
	}
}
int main(int argc, char **argv)
{
    pid_t pid;
    int sockfd,confd;
    char buffer[SIZE],buf[SIZE]; 
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    struct hostent *host;
    short port;

	//线程标识号
	pthread_t thread_recv,thread_send;
	void *status;
	int ret;
    //四个参数
    if(argc!=3) 
    { 
        fprintf(stderr,"Usage:%s hostname name\a\n",argv[0]); 
        exit(1); 
    } 
    //使用hostname查询host 名字 
    if((host=gethostbyname(argv[1]))==NULL) 
    { 
        fprintf(stderr,"Gethostname error\n"); 
        exit(1); 
    } 
	//port=atoi(argv[2]);
	strcpy(name,argv[2]);
	printf("name is :%s\n",name);
	/*客户程序开始建立 sockfd描述符 */ 
    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0) 
    { 
		perror("socket");
		exit(-1);
    } 
    printf("Socket successful!\n");

    /*客户程序填充服务端的资料 */ 
    bzero(&server_addr,sizeof(server_addr)); // 初始化,置0
    server_addr.sin_family=AF_INET;          // IPV4
    server_addr.sin_port=htons(SERV_PORT);  // (将本机器上的short数据转化为网络上的short数据)端口号
    server_addr.sin_addr=*((struct in_addr *)host->h_addr); // IP地址
    /* 客户程序发起连接请求 */ 
    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr)) < 0) 
    {
		perror("connect");
		exit(-1); 
    }
	printf("Connect successful!\n");
    /*将客户端的名字发送到服务器端*/
    send(sockfd,name,20,0);
	
	//创建线程行读写操作/
	ret = pthread_create(&thread_recv, NULL, pthread_recv, &sockfd);//用于接收信息
	if(ret != 0)
	{
		perror("Create thread_recv fail!");
		exit(-1);
	}
	ret = pthread_create(&thread_send, NULL, pthread_send, &sockfd);//用于发送信息
	if(ret != 0)
	{
		perror("Create thread_send fail!");
		exit(-1);
	}
	printf("wait for thread_recv \n");
	pthread_join(thread_recv, &status);
	printf("wait for thread_send \n");
	pthread_join(thread_send, &status);
	printf("close sockfd \n");
	close(sockfd);
    return 0;    
}
