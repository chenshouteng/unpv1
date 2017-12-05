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
#define SIZE 1024

#define	SERV_PORT		 9878

int main(int argc, char **argv)
{
    pid_t pid;
    int sockfd,confd;
    char buffer[SIZE],buf[SIZE]; 
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    struct hostent *host;
    short port;
    char *name;  
    //�ĸ�����
    if(argc!=3) 
    { 
        fprintf(stderr,"Usage:%s hostname name\a\n",argv[0]); 
        exit(1); 
    } 
    //ʹ��hostname��ѯhost ���� 
    if((host=gethostbyname(argv[1]))==NULL) 
    { 
        fprintf(stderr,"Gethostname error\n"); 
        exit(1); 
    } 
	//port=atoi(argv[2]);
	name=argv[2];
	/*�ͻ�����ʼ���� sockfd������ */ 
    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0) 
    { 
		perror("socket");
		exit(-1);
    } 
    printf("Socket successful!\n");

    /*�ͻ�����������˵����� */ 
    bzero(&server_addr,sizeof(server_addr)); // ��ʼ��,��0
    server_addr.sin_family=AF_INET;          // IPV4
    server_addr.sin_port=htons(SERV_PORT);  // (���������ϵ�short����ת��Ϊ�����ϵ�short����)�˿ں�
    server_addr.sin_addr=*((struct in_addr *)host->h_addr); // IP��ַ
    /* �ͻ��������������� */ 
    if(confd=connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr)) < 0) 
    {
		perror("connect");
		exit(-1); 
    }
	printf("Connect successful!\n");
    /*���ͻ��˵����ַ��͵���������*/
    send(sockfd,name,20,0);
     /*�����ӽ��̣����ж�д����*/
    pid = fork();//�����ӽ���
    while(1)
    {
       /*���������ڷ�����Ϣ*/
	   if(pid > 0)
	   {
          /*ʱ�亯��*/
           struct tm *p_curtime ;
		   time_t timep;
           time(&timep);
           p_curtime = localtime(&timep);
           strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", p_curtime);
           /*���ʱ��Ϳͻ��˵�����*/
           strcat(buffer," \n\t�ǳ� ->");
           strcat(buffer,name);
           strcat(buffer,":\n\t\t  ");
           memset(buf,0,SIZE);
           fgets(buf,SIZE,stdin);//��ȡ�û��������Ϣ
		   /*�Կͻ��˳�����й���*/
           if(strncmp("e",buf,1)==0)
           {
				printf("�ÿͻ�������...\n");
				strcat(buffer,"�˳������ң�");
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
        else if(pid == 0)
        {
            /*�ӽ������ڽ�����Ϣ*/
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
	close(sockfd);
    return 0;    
}
