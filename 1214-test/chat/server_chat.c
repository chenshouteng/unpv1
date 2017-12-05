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
#define BACKLOG 3
int sockfd;
int fd[BACKLOG];
int i=0;
/*********�׽���������*******/
int get_sockfd()
{
    struct sockaddr_in server_addr;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
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
    if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(-1);  
    }
	
    /*�󶨷�������ip�ͷ������˿ں�*/
    if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)     
    {      
		perror("bind");
		exit(-1);   
    }
	printf("Bind successful!\n");
	/* �����������ӵ����ͻ����� */     
	if(listen(sockfd,BACKLOG)==-1)     
	{
		perror("bind");
		exit(-1); 
	}	
	printf("Listening.....\n");
	return sockfd;
}

/*��������洢��*/
int shmid_create()
{    
	int shmid;
	if((shmid = shmget(IPC_PRIVATE,SIZE_SHMADD,0777)) < 0)
	{
		perror("shmid error!");
		exit(-1);	
	}
    printf("shmid success!\n");
    return shmid;
}
int main(int argc, char **argv)
{
	char shmadd_buffer[SIZE_SHMADD],buffer[SIZE];
	struct sockaddr_in client_addr;
    int sin_size;
    pid_t ppid,pid;
	int new_fd;
    int shmid;
    char *shmadd;
	int i = 0;
    /***********�����ڴ�**************/
    shmid = shmid_create();
    //ӳ�乲���ڴ�
    shmadd = shmat(shmid, 0, 0);
    /*****�����׽���������***********/ 
    int sockfd = get_sockfd();
    /*ѭ�����տͻ���*/
    while(1)
    {
		//����������,ֱ���ͻ����������� 
        sin_size=sizeof(struct sockaddr_in);
        if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)         
        {
			perror("accept");
			exit(-1);
		}
		printf("Accept successful!\n");
		fd[i++] = new_fd;
		printf("\n�������˿ͻ���%d : %s:%d \n",i , inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		//printf("�������˿ͻ���%d:%d :%d \n",i , ntohs(client_addr.sin_port));
        /*�ѽ��淢�͸��ͻ���*/
		memset(buffer,0,SIZE);
		//strcpy(buffer,"\n������������������������������������Welecom come char ����������������������������������������������\n");
		strcpy(buffer,"\n###Welecom come char ###\n");
		send(new_fd,buffer,SIZE,0);
		//�����ӽ��̿ͻ���
		ppid = fork();
		if(ppid == 0)//child process
		{
			//��������¿ͻ����͸��������ߵĿͻ���/
			recv(new_fd,buffer,SIZE,0);
			strcat( buffer," enter chat....");  
			for(i=0;i<BACKLOG;i++)
			{
				if(fd[i]!=-1)
				{  
					send(fd[i],buffer,strlen(buffer),0);  
				}  
            }
			//�����ӽ��̽��ж�д����/
			pid = fork();    
			while(1)
			{
				if(pid > 0)
				{
					//���������ڽ�����Ϣ/
					memset(buffer,0,SIZE);
					if((recv(new_fd,buffer,SIZE,0)) <= 0)
					{
						close(new_fd);
						exit(1);
					}
					printf("in parent process\n");
					memset(shmadd, 0, SIZE_SHMADD);
					strncpy(shmadd, buffer, SIZE_SHMADD);//���������Ŀͻ�����Ϣ���빲���ڴ���
					printf(" %s\n",buffer);
				}
				if(pid == 0)
				{
					//�ӽ������ڷ�����Ϣ/
					sleep(1);//��ִ�и�����
					
					if(strcmp(shmadd_buffer,shmadd) != 0)
					{
						strcpy(shmadd_buffer,shmadd);
						if(new_fd  > 0)
						{
							if(send(new_fd,shmadd,strlen(shmadd),0) == -1)
							{
								perror("send");
							}
							printf("in child process\n");
							//memset(shmadd, 0, SIZE_SHMADD);
							//strcpy(shmadd,shmadd_buffer);                
						}
					}
				}
			}
		} 
   }
   free(buffer);
   close(new_fd);
   close(sockfd);
   return 0;
}