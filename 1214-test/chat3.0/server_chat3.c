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

int listenfd;
int connfd[LISTEN_MAX];


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

void* pthread_handle(void * arg)
{
    int index,i;
    index = *(int *)arg;
    printf("in pthread_recv,index = %d,connfd = %d\n",index,connfd[index]);
	char buffer[SIZE];
	while(1)
	{
		//用于接收信息
		memset(buffer,0,SIZE);
		//printf("111 index = %d,connfd = %d\n",index,connfd[index]);
		if((recv(connfd[index],buffer,SIZE,0)) <= 0)
		{
			close(connfd[index]);
			pthread_exit(0);
		}
		printf(" %s\n",buffer);
		//printf(" begin send\n");
		for(i = 0; i < LISTEN_MAX ; i++)  
		{
			if(connfd[i] != -1)  
			{
				//printf("send to %d\n",connfd[i]);
				if(send(connfd[i],buffer,strlen(buffer),0) == -1)
				{
					perror("send");
					pthread_exit(0);
				} 
			}  
		}  

	}
}
void quit()  
{  
    char msg[10];
	int i = 0;
    while(1)  
    {
		printf("please enter 'Q' to quit server!\n");
        scanf("%s",msg);  
        if(strcmp("Q",msg)==0)  
        {  
            printf("now close server\n");  
            close(listenfd); 
			for(i = 0; i < LISTEN_MAX ; i++)  
			{  
				if(connfd[i] != -1)  
				{
					close(connfd[i]); 
				}  
			} 		
            exit(0);  
        }  
    }  
}  
int main(int argc, char **argv)
{
	struct sockaddr_in client_addr;
    int sin_size;
    pid_t ppid,pid;

	int num = 0,i = 0,ret;
	//线程标识号
	pthread_t thread_server_close,thread_handle;
	//unsigned char buffer[SIZE];
	char buffer[SIZE];
    //创建套接字描述符
    int listenfd = get_sockfd();
	//记录空闲的客户端的套接字描述符（-1为空闲）  
    for(i = 0 ; i < LISTEN_MAX; i++)  
    {  
        connfd[i]=-1;  
    }

	//创建一个线程，对服务器程序进行管理（关闭）  
    ret = pthread_create(&thread_server_close,NULL,(void*)(&quit),NULL); 
	if(ret != 0)
	{
		perror("Create pthread_handle fail!");
		exit(-1);
	}
    while(1)
    {
        for(i=0;i < LISTEN_MAX;i++)  
        {  
			printf("i == %d\n",i);
            if(connfd[i]==-1)//表示套接字容器空闲，可用
            {  
                break;  
            }
        } 
		printf("before accept i == %d\n",i);
		//服务器阻塞,直到客户程序建立连接 
        sin_size=sizeof(struct sockaddr_in);
        if((connfd[i]=accept(listenfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)         
        {
			perror("accept");
			exit(-1);//要continue还是exit，再考虑
		}

		printf("Accept successful!\n");
		//fd[num++] = connfd;
		printf("connect to client %d : %s:%d \n",num , inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /*把界面发送给客户端*/
		memset(buffer,0,SIZE);
		strcpy(buffer,"\n------------------Welecom come char------------------------\n");
		//printf("buffer :%s",buffer);
		send(connfd[i],buffer,SIZE,0);
		//创建子进程客户端
		//ppid = fork();
		//if(ppid == 0)//child process
		//{
		//将加入的新客户发送给所有在线的客户端/
		printf("before recv\n");
		recv(connfd[i],buffer,SIZE,0);
		printf("after recv\n");
		strcat( buffer," enter chat...."); 
		int j;
		for(j = 0; j < LISTEN_MAX; j++)
		{
			if(connfd[j] != -1)
			{
				printf("j == %d\n",j);
				send(connfd[j],buffer,strlen(buffer),0);  
			}
		}
		//printf("before pthread_create i == %d\n",i);
		int socked_index = i;//这里避免线程还未创建完成，i的值可能会被while循环修改
		//创建线程行读写操作/
		ret = pthread_create(&thread_handle, NULL, pthread_handle, &socked_index);//用于接收信息
		if(ret != 0)
		{
			perror("Create pthread_handle fail!");
			exit(-1);
		}
   }

   return 0;
}