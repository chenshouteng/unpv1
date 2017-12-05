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
//����һ���̹߳�����Դ
pthread_mutex_t mutex;

/*********�׽���������*******/
int get_sockfd()
{
    struct sockaddr_in server_addr;
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(-1);
	}
    printf("Socket successful!\n");
	/*sockaddr�ṹ */ 
    bzero(&server_addr,sizeof(struct sockaddr_in)); 
    server_addr.sin_family=AF_INET;                
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY); 
    server_addr.sin_port=htons(PORT);  
	
 // �����׽���ѡ������ַʹ�ô���Ϊ�������ַ���ã����������Ͳ�����on��Ϊ 1 ����Ȼ��������Ϊ 0 ����ֹ��ַ���ã�
    int on=1;  
    if((setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(-1);  
    }
	
    /*�󶨷�������ip�ͷ������˿ں�*/
    if(bind(listenfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)     
    {      
		perror("bind");
		exit(-1);   
    }
	printf("Bind successful!\n");
	/* �����������ӵ����ͻ����� */     
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
		//���ڽ�����Ϣ
		memset(buffer,0,SIZE);
		if((recv(connfd,buffer,SIZE,0)) <= 0)
		{
			close(connfd);
			pthread_exit(0);
		}
		printf("in parent pthread\n");
		//����������
		if(pthread_mutex_lock(&mutex)!=0)
		{
			perror("pthread_mutex_lock");
		}
		memset(g_recv_buff, 0, SIZE);
		strncpy(g_recv_buff, buffer, SIZE);//���������Ŀͻ�����Ϣ���빲���ڴ���
		//����
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
			sleep(1);//�ȴ�һ�£��ӳٸ�ֵ��g_send_buff��Ȼ�����߳̽��������
			//����������
			if(pthread_mutex_lock(&mutex)!=0)
			{
				perror("pthread_mutex_lock");
			}
			memset(g_send_buff, 0, SIZE);
			strcpy(g_send_buff,g_recv_buff);
			//����
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
	//�̱߳�ʶ��
	pthread_t thread_send,thread_recv;
	//unsigned char buffer[SIZE];
	char buffer[SIZE];
    //�����׽���������
    int listenfd = get_sockfd();

    while(1)
    {
		//����������,ֱ���ͻ����������� 
        sin_size=sizeof(struct sockaddr_in);
        if((connfd=accept(listenfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)         
        {
			perror("accept");
			exit(-1);
		}
		printf("Accept successful!\n");
		fd[num++] = connfd;
		printf("connect to client %d : %s:%d \n",num , inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /*�ѽ��淢�͸��ͻ���*/
		memset(buffer,0,SIZE);
		strcpy(buffer,"\n--------------Welecom come char--------------\n");
		//strcpy(buffer,"������������������������������������Welecom come chat ����������������������������������������������\n");
		printf("buffer :%s",buffer);
		send(connfd,buffer,SIZE,0);
		//�����ӽ��̿ͻ���
		//ppid = fork();
		//if(ppid == 0)//child process
		//{
			//��������¿ͻ����͸��������ߵĿͻ���/
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

		//�����߳��ж�д����/
		ret = pthread_create(&thread_recv, NULL, pthread_recv, &connfd);//���ڽ�����Ϣ
		if(ret != 0)
		{
			perror("Create thread_recv fail!");
			exit(-1);
		}
		ret = pthread_create(&thread_send, NULL, pthread_send, &connfd);//���ڷ�����Ϣ
		if(ret != 0)
		{
			perror("Create thread_send fail!");
			exit(-1);
		}
   }
   //���ﲢ�����ߵ�����
   close(connfd);
   close(listenfd);
   return 0;
}