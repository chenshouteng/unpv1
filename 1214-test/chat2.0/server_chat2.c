#include<stdio.h>   
#include<stdlib.h>
#include<sys/types.h> 
#include<sys/stat.h>
#include<netinet/in.h>  
#include<sys/socket.h> 
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/ipc.h>
#include<errno.h>
#include<sys/shm.h>
#include<time.h>
#include<pthread.h>

#include <arpa/inet.h>

#define PORT 9878
#define SIZE 1024
#define SIZE_SHMADD 2048
#define LISTEN_MAX 10
char g_recv_buff[SIZE],g_send_buff[SIZE];

int listenfd;
int fd[LISTEN_MAX];
//定义一个线程共享资源
pthread_mutex_t mutex;

/*********套接字描述符*******/
int get_sockfd()
{
    struct sockaddr_in server_addr;
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(-1);
	}
    printf("Socket successful!\n");
	/*sockaddr结构 */ 
    bzero(&server_addr,sizeof(struct sockaddr_in)); 
    server_addr.sin_family=AF_INET;                
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY); 
    server_addr.sin_port=htons(PORT);  
	
 // 设置套接字选项避免地址使用错误，为了允许地址重用，我设置整型参数（on）为 1 （不然，可以设为 0 来禁止地址重用）
    int on=1;  
    if((setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(-1);  
    }
	
    /*绑定服务器的ip和服务器端口号*/
    if(bind(listenfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)     
    {      
		perror("bind");
		exit(-1);   
    }
	printf("Bind successful!\n");
	/* 设置允许连接的最大客户端数 */     
	if(listen(listenfd,LISTEN_MAX)==-1)     
	{
		perror("bind");
		exit(-1); 
	}	
	printf("Listening.....\n");
	return listenfd;
}

//char g_recv_buff[SIZE],g_send_buff[SIZE];
void* pthread_recv(void * arg)
{
    int connfd;
    connfd = *(int *)arg;
    printf("in pthread_recv,connfd = %d\n",connfd);
	char buffer[SIZE];
	while(1)
	{
		//用于接收信息
		memset(buffer,0,SIZE);
		if((recv(connfd,buffer,SIZE,0)) <= 0)
		{
			close(connfd);
			pthread_exit(0);
		}
		printf("in parent pthread\n");
		//互斥锁上锁
		if(pthread_mutex_lock(&mutex)!=0)
		{
			perror("pthread_mutex_lock");
		}
		memset(g_recv_buff, 0, SIZE);
		strncpy(g_recv_buff, buffer, SIZE);//将缓存区的客户端信息放入共享内存里
		//解锁
		if(pthread_mutex_unlock(&mutex)!=0)
		{
			perror("pthread_mutex_unlock");
		}
		printf(" %s\n",buffer);
	}
}
void* pthread_send(void * arg)
{
    int connfd;
    connfd = *(int *)arg;
    printf("in pthread_send,connfd = %d\n",connfd);
	while(1)
	{

		if(strcmp(g_recv_buff,g_send_buff) != 0)
		{
			sleep(1);//等待一下，延迟赋值给g_send_buff不然其他线程进不来这儿
			//互斥锁上锁
			if(pthread_mutex_lock(&mutex)!=0)
			{
				perror("pthread_mutex_lock");
			}
			memset(g_send_buff, 0, SIZE);
			strcpy(g_send_buff,g_recv_buff);
			//解锁
            if(pthread_mutex_unlock(&mutex)!=0)
            {
                perror("pthread_mutex_unlock");
            }
			if(send(connfd,g_send_buff,strlen(g_send_buff),0) == -1)
			{
				perror("send");
				 pthread_exit(0);
			}
			printf("in child process\n");            
		}	
	}
}
int main(int argc, char **argv)
{

	struct sockaddr_in client_addr;
    int sin_size;
    pid_t ppid,pid;
	int connfd;
    //int shmid;
    //char *shmadd;
	int num = 0,i = 0,ret;
	//线程标识号
	pthread_t thread_send,thread_recv;
	//unsigned char buffer[SIZE];
	char buffer[SIZE];
    //创建套接字描述符
    int listenfd = get_sockfd();

    while(1)
    {
		//服务器阻塞,直到客户程序建立连接 
        sin_size=sizeof(struct sockaddr_in);
        if((connfd=accept(listenfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)         
        {
			perror("accept");
			exit(-1);
		}
		printf("Accept successful!\n");
		fd[num++] = connfd;
		printf("connect to client %d : %s:%d \n",num , inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /*把界面发送给客户端*/
		memset(buffer,0,SIZE);
		strcpy(buffer,"\n--------------Welecom come char--------------\n");
		//strcpy(buffer,"——————————————————Welecom come chat ———————————————————————\n");
		printf("buffer :%s",buffer);
		send(connfd,buffer,SIZE,0);
		//创建子进程客户端
		//ppid = fork();
		//if(ppid == 0)//child process
		//{
			//将加入的新客户发送给所有在线的客户端/
		printf("before recv\n");
		recv(connfd,buffer,SIZE,0);
		printf("after recv\n");
		strcat( buffer," enter chat....");  
		for(i = 0; i < LISTEN_MAX; i++)
		{
			if(fd[i] > 0)
			{
				printf("i == %d\n",i);
				send(fd[i],buffer,strlen(buffer),0);  
			}
		}

		//创建线程行读写操作/
		ret = pthread_create(&thread_recv, NULL, pthread_recv, &connfd);//用于接收信息
		if(ret != 0)
		{
			perror("Create thread_recv fail!");
			exit(-1);
		}
		ret = pthread_create(&thread_send, NULL, pthread_send, &connfd);//用于发送信息
		if(ret != 0)
		{
			perror("Create thread_send fail!");
			exit(-1);
		}
   }
   //这里并不会走到这里
   close(connfd);
   close(listenfd);
   return 0;
}