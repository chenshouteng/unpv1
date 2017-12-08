
2017年12月4号

ubuntu安装git命令
sudo apt-get install git

一、生成ssh密钥对
终端执行命令 ssh-keygen -t rsa -C "xxx@xxx.com"
然后3次回车，最终看到一个方框，如下图。这时已经在/home/ubuntu/.ssh/ 目录下生成公钥id_rsa.pub和私钥id_rsa。
ubuntu@ubuntu:~$ ssh-keygen -t rsa -C "xxx@xxx.com" 
Generating public/private rsa key pair.
Enter file in which to save the key (/home/ubuntu/.ssh/id_rsa): 
Created directory '/home/ubuntu/.ssh'.
Enter passphrase (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in /home/ubuntu/.ssh/id_rsa.
Your public key has been saved in /home/ubuntu/.ssh/id_rsa.pub.
The key fingerprint is:
b3:f8:bd:14:c8:9c:3d:fe:c1:f6:13:f4:4b:85:f8:0f xxx@xxx.com
The key's randomart image is:
+--[ RSA 2048]----+
|                 |
|                 |
|             . . |
|       o +  . o .|
|        S +  o ..|
|       . + +  E..|
|      . . o + .+.|
|       . o o o...|
|        . o.. .. |
+-----------------+
查看公钥，然后把公钥添加到https://github.com/settings/ssh/new 页面，Title随便起，方便自己辨识，Key填写获取到的公钥。
ubuntu@ubuntu:~$ cat .ssh/id_rsa.pub 
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDAJaYOXwqyPESeaxlxJdEKCubSgsRK0kDrfcnYUr2eJ6i3ip9Tb1DuP1PXTnwfbsQJigNuUTrKEmueDL0nDEsGmnIe2GyWtzu9namg4HxRdjGZGC6t1D4d+ASDz+k4RMEy8ibPayW0w9P0qqKuUyrmSB2CrycfnwP/QTFj2YWDHSZNkHXpDvchEJT3Ga7S4DOYpgTvConF0JjVPfxvvTUqigtyxqdJwGpgeOwyNjVgJD30x1c605cMEdknHLANR0FoAM9gEjQs6NHw6qW4vd4kpJac93Kc7NosMDcNoFTBwUgxL8JKmTWqO8V+bb5zF7ANLxcW2pXZoYgnoMN4egqB xxx@xxx.com
输入命令 ssh -T git@github.com 测试是否成功。执行后再yes，最后一句出现You've successfully authenticated 表明成功了。
ubuntu@ubuntu:~$ ssh -T git@github.com
The authenticity of host 'github.com (192.30.255.112)' can't be established.
RSA key fingerprint is 16:27:ac:a5:76:28:2d:36:63:1b:56:4d:eb:df:a6:48.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'github.com,192.30.255.112' (RSA) to the list of known hosts.
Hi chenshouteng! You've successfully authenticated, but GitHub does not provide shell access.

接着需要配置你的标识(Your Identity) 
安装Git后首先要做的事情是设置你的用户名称和e-mail地址。每次Git提交都会使用该信息。例如，当你提交文件修改后，
用git log 命令的时候，会显示是jack和jack@qq.com提交了此修改。
 　　git config --global user.name "jack"
 　　git config --global user.email jack@qq.com 

用git clone命令把远程的项目克隆到当前目录。
ubuntu@ubuntu:~/git_project/unp$ git clone https://github.com/chenshouteng/unpv1.git
fatal: could not create work tree dir 'unpv1'.: Permission denied
如上所示，如果出现上面这种情况，那是因为你在当前目录下没有写权限，用chmod修改一下权限即可。
sudo chmod o+w dirname(你想要存放项目的目录)

接下来可以发现git clone成功了。这里我切换了root权限，需要用git config重新配置，读者可不用理会，不用更改到root权限操作。
root@ubuntu:/home/ubuntu/git_project/unp# git clone 远程项目地址
Cloning into 'unpv1'...
remote: Counting objects: 7, done.
remote: Compressing objects: 100% (3/3), done.
remote: Total 7 (delta 0), reused 4 (delta 0), pack-reused 0
Unpacking objects: 100% (7/7), done.
Checking connectivity... done.
root@ubuntu:/home/ubuntu/git_project/unp# ls
unpv1
root@ubuntu:/home/ubuntu/git_project/unp# cd unpv1/
root@ubuntu:/home/ubuntu/git_project/unp/unpv1# ls
client  README.md

我们创建server目录，然后在server目录下添加server.c文件，里面没啥内容，主要为了测试把文件push到远端。
root@ubuntu:/home/ubuntu/git_project/unp/unpv1# ls
client  README.md  server
root@ubuntu:/home/ubuntu/git_project/unp/unpv1# git add server
root@ubuntu:/home/ubuntu/git_project/unp/unpv1# git commit -m "test"
[master 99b8068] test
 1 file changed, 1 insertion(+)
 create mode 100644 server/server.c


root@ubuntu:/home/ubuntu/git_project/unp/unpv1# git push
warning: push.default is unset; its implicit value is changing in
Git 2.0 from 'matching' to 'simple'. To squelch this message
and maintain the current behavior after the default changes, use:

  git config --global push.default matching

To squelch this message and adopt the new behavior now, use:

  git config --global push.default simple

When push.default is set to 'matching', git will push local branches
to the remote branches that already exist with the same name.

In Git 2.0, Git will default to the more conservative 'simple'
behavior, which only pushes the current branch to the corresponding
remote branch that 'git pull' uses to update the current branch.

See 'git help config' and search for 'push.default' for further information.
(the 'simple' mode was introduced in Git 1.7.11. Use the similar mode
'current' instead of 'simple' if you sometimes use older versions of Git)

Username for 'https://github.com': chenshouteng
Password for 'https://chenshouteng@github.com': 
Counting objects: 5, done.
Compressing objects: 100% (2/2), done.
Writing objects: 100% (4/4), 355 bytes | 0 bytes/s, done.
Total 4 (delta 0), reused 0 (delta 0)
To https://github.com/chenshouteng/unpv1.git
   2c9e1db..99b8068  master -> master
   

git remote -v   
git remote add origin git@github.com:chenshouteng/unpv1.git


Clone with HTTPS 
Use Git or checkout with SVN using the web URL.
如果我们git clone https://github.com/chenshouteng/unpv1.git
那么git remote -v 的时候，就会走https，git push的时候需要输入密码。想要不输入密码就需要执行下列命令修改。
把现在的origin的方式删掉
git remote rm origin
第二句是增加新的origin
git remote add origin git@github.com:chenshouteng/unpv1.git
 
ubuntu@ubuntu:~/git_project/unp_ubuntu/unpv1_test2/unpv1$ git remote -v
origin  https://github.com/chenshouteng/unpv1.git (fetch)
origin  https://github.com/chenshouteng/unpv1.git (push)
ubuntu@ubuntu:~/git_project/unp_ubuntu/unpv1_test2/unpv1$ git remote rm origin
ubuntu@ubuntu:~/git_project/unp_ubuntu/unpv1_test2/unpv1$ git remote -v
ubuntu@ubuntu:~/git_project/unp_ubuntu/unpv1_test2/unpv1$ git remote add origin git@github.com:chenshouteng/unpv1.git
ubuntu@ubuntu:~/git_project/unp_ubuntu/unpv1_test2/unpv1$ git remote -v
origin  git@github.com:chenshouteng/unpv1.git (fetch)
origin  git@github.com:chenshouteng/unpv1.git (push)


Clone with SSH 
如果我们git clone git@github.com:chenshouteng/unpv1.git ，那么就会走ssh校验，不用做任何修改，就可以git push 不输入密码，
前提是我们有把公钥配置到git服务器上。
ubuntu@ubuntu:~/git_project/unp_ubuntu/unpv1$ git remote -v
origin  git@github.com:chenshouteng/unpv1.git (fetch)
origin  git@github.com:chenshouteng/unpv1.git (push)


git log  查看 提交历史
我们常用 -p 选项展开显示每次提交的内容差异，后面加数字n则表示仅显示最近的n次更新，命令如下
git log -p -2
	

git diff
此命令比较的是工作目录(Working tree)和暂存区域快照(index)之间的差异
修改了文件之后，要提交之前，可以先执行git diff，查看自己改动了哪些文件哪些内容。
git diff 后面跟上commit id，可以查看两个版本之后的差异

git status命令可以列出当前目录所有还没有被git管理的文件和被git管理且被修改但还未提交(git commit)的文件。

git pull：相当于是从远程获取最新版本并merge到本地

总结一下
修改文件：
          git add .
          git commit -am "我执行了修改"
          git push
三、删除文件：
           rm  -rf a.txt --删除a.txt文件
           git rm a.txt --从托管区删除a.txt文件
           git commit -m "delete" --提交，此时从本地库删除a.txt文件
           git push     -- 更新到远程库
		   
git reset --hard xx -- 强制退回到某次的修改





samba配置：
root@ubuntu:/etc/samba# vim smb.conf 
在文件最后加上
[ubuntu]
  browseable =yes
  writeable =yes
  path =/home/ubuntu
  valid users =ubuntu
配置samba 账号的密码：
root@ubuntu:/home# smbpasswd -a ubuntu
New SMB password:
Retype new SMB password:
Added user ubuntu.
重新启动smbd服务
root@ubuntu:/home# /etc/init.d/smbd restart
smbd stop/waiting
smbd start/running, process 4541


2017年12月5号
socket编程小问题：地址已经被使用——Address already in use





先关闭服务器而不关闭客户端，然后再重新启动服务器，就会出现如下错误，
bind: Address already in use
这是由于套接字处于TIME_WAIT状态引起的，这个时间是几分钟，过后再重新启动服务器就没问题了。
有时候，我们在调试程序的时候，想要避免这种情况，可以
在bind前加上这两句话可以避免
    // 设置套接字选项避免地址使用错误  
    int on=1;  
    if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    }  
	
需要考虑的是，当客户端断开的时候，服务器线程该怎么处理。应该考虑回收资源。
共享内存访问需要加锁

2017年12月6号
以后再增加新功能，参考：
http://blog.csdn.net/ccj2020/article/details/7838910

代理服务器实现，参考：
http://blog.csdn.net/he11o_liu/article/details/76796510



TCP网络编程之echo回射程序
TCP网络编程有三个例子最值得学习研究，分别是echo、chat、proxy，都是长连接协议。
接下去，有空会把这几个例子都实现。
本节用一个简单的例子来讲TCP客户/服务器程序框架，这也是echo的实现。
客户从标准输入键入一行文本，并发送给服务器。
服务器接收到文本之后回射给客户端。
客户端接收到服务器的文本，把它显示到标准输出上。

尽管下列实现代码很简单，但是它已经阐述了基本的tcp客户/服务器的框架，
想要实现任何复杂的程序都可以以这个程序作为基本框架来开发。
比如做成一问一答的方式，收到的请求和发送响应的内容不一样，这时候要考虑打包与拆包格式的设计，进一步还可以写简单的HTTP服务。
以下贴出源码。后续以tcp socket的框架写个简单功能的聊天室。

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>

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

测试结果如下：
chenting@soft4dept-ubuntu:~/test/1214-test$ ./server_echo 
create socket success!
Bind success!
begin accept!
begin fork!
begin accept!
server begin recv
server send
chenting@soft4dept-ubuntu:~/test/1214-test$ ./client_echo 192.168.65.1
helloworld
sendline : helloworld

recvline : helloworld

helloworld


socket编程小问题：地址已经被使用——Address already in use




在调试程序的时候，我发现如果服务器被断开了，再重新启动，会出现如下错误：
bind: Address already in use
查了之后，才知道这是由于套接字处于TIME_WAIT状态引起的，这个时间是几分钟，过后再重新启动服务器就没问题了。
有时候，我们在调试程序的时候，为了允许地址重用，可以在bind前加上这两句话可以避免这种状况。设置整型参数（on）为 1 （不然，可以设为 0 来禁止地址重用）
    int on=1;  
    if((setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(-1);  
    }

	
TCP网络编程之chat聊天室

这一节我们再讲一个tcp长连接的例子，实现网络聊天室的基本功能。
聊天室的基本原理：采用Client/Server TCP架构，客户端发送消息给服务器，服务器再把消息转发给所有的客户端。

聊天室功能清单，总结的很好，来自网络http://blog.csdn.net/ccj2020/article/details/7838910
实现目标
一个在Linux下可以使用的聊天软件,要求至少实现如下功能:
1. 采用Client/Server架构
2. Client A 登陆聊天服务器前,需要注册自己的ID和密码
3. 注册成功后,Client A 就可以通过自己的ID和密码登陆聊天服务器
4. 多个Client X 可以同时登陆聊天服务器之后,与其他用户进行通讯聊天
5. Client A成功登陆后可以查看当前聊天室内其他在线用户Client x
6. Client A可以选择发消息给某个特定的Client X,即”悄悄话”功能
7. Client A 可以选择发消息全部的在线用户,即”群发消息”功能
8. Client A 在退出时需要保存聊天记录
9. Server端维护一个所有登陆用户的聊天会的记录文件,以便备查
可以选择实现的附加功能:
1. Server可以内建一个特殊权限的账号admin,用于管理聊天室
2. Admin可以将某个Client X “提出聊天室”
3. Admin可以将某个Client X ”设为只能旁听,不能发言”
4. Client 端发言增加表情符号,可以设置某些自定义的特殊组合来表达感情.如输入:),则会自动发送”XXX向大家做了个笑脸”
5. Client段增加某些常用话语,可以对其中某些部分进行”姓名替换”,例如,输入/ClientA/welcome,则会自动发送”ClientA 大侠,欢迎你来到咱们的聊天室”
附加功能：
文件传输

这里我只完成了最基本的功能4，多个客户同时聊天，这也是聊天室的核心功能，其它功能以后再一一实现。

程序的实现是采用Client/Server TCP架构，服务器负责监听客户端的连接。
当有客户端连接上服务器时，服务器会专门为连接上的客户端开一个线程，用来接收客户端发送过来的消息并把此消息转发给所有的客户端。
此外，程序还开了一个线程专门处理关闭服务器的线程，当我们在终端输入字符'Q'时，服务器将关闭所有的连接并退出进程。
程序基本架构：
主线程：监听来自客户端的连接，如果没有连接，则阻塞在accept函数。
pthread_handle线程处理函数：接收客户发来的消息并群发出去。
quit线程处理函数：可实现通过终端随时关闭服务器。


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

//套接字描述符
int get_sockfd()
{
    struct sockaddr_in server_addr;
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(-1);
	}
    printf("Socket successful!\n");
	//sockaddr结构 
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
	
    //绑定服务器的ip和服务器端口号
    if(bind(listenfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)     
    {      
		perror("bind");
		exit(-1);   
    }
	printf("Bind successful!\n");
	//设置允许连接的最大客户端数     
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
		if((recv(connfd[index],buffer,SIZE,0)) <= 0)
		{
			close(connfd[index]);
			pthread_exit(0);
		}
		printf(" %s\n",buffer);
		for(i = 0; i < LISTEN_MAX ; i++)  
		{
			if(connfd[i] != -1)  
			{
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
		printf("connect to client %d : %s:%d \n",num , inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        //把界面发送给客户端
		memset(buffer,0,SIZE);
		strcpy(buffer,"\n------------------Welecom come char------------------------\n");
		send(connfd[i],buffer,SIZE,0);

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
		int socked_index = i;//这里避免线程还未创建完成，i的值可能会被while循环修改
		//创建线程行读写操作
		ret = pthread_create(&thread_handle, NULL, pthread_handle, &socked_index);//用于接收信息
		if(ret != 0)
		{
			perror("Create pthread_handle fail!");
			exit(-1);
		}
   }
   return 0;
}
这里需要注意的点：
原来程序写成了下面这样，把i作为参数传递给线程函数thread_handle。
	ret = pthread_create(&thread_handle, NULL, pthread_handle, &i);//用于接收信息
	if(ret != 0)
	{
		perror("Create pthread_handle fail!");
		exit(-1);
	}
后来调试程序的时候发现问题了，就改成了先把i变量赋值给socked_index，然后再把socked_index传递给线程函数thread_handle。
这样做有什么区别呢？读者可以思考一下。
		int socked_index = i;
		ret = pthread_create(&thread_handle, NULL, pthread_handle, &socked_index);//用于接收信息
		if(ret != 0)
		{
			perror("Create pthread_handle fail!");
			exit(-1);
		}
原因是，加入我们用变量i传入线程函数，那么在主线程while循环中，在变量i还未传入的时候，变量i被修改了，造成程序出错。
可以理解线程创建需要一定的时间，但是此时变量i会被主线程修改。


客户端程序的基本框架：
主线程主动和服务器建立连接，然后创建两个线程，一个用于发送消息，一个用于接收消息。
pthread_send线程处理函数：获取客户的输入和当前时间，如果输入是'Q'字符，则退出，否则就把消息发送给服务器。
pthread_recv线程处理函数：接收来自服务器的消息，并打印到终端。


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
		if(strncmp("Q",buf,1)==0)
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

ubuntu:~/test/1214-test/chat3.0$ ./server_chat3
Socket successful!
Bind successful!
Listening.....
i == 0
before accept i == 0
please enter 'Q' to quit server!
Accept successful!
connect to client 0 : 192.168.65.1:44408 
before recv
after recv
j == 0
i == 0
i == 1
before accept i == 1
in pthread_recv,index = 0,connfd = 4
Accept successful!
connect to client 0 : 192.168.65.1:44409 
before recv
after recv
j == 0
j == 1
i == 0
i == 1
i == 2
before accept i == 2
in pthread_recv,index = 1,connfd = 5
 2017/12/06 16:14:27 
        昵称 ->xiaoming:
                  hello xiaohong

 2017/12/06 16:14:37 
        昵称 ->xiaohong:
                  hello xiaoming
ubuntu:~/test/1214-test/chat3.0$ ./client_chat3 192.168.65.1 xiaoming
name is :xiaoming
Socket successful!
Connect successful!

------------------Welecom come char------------------------

wait for thread_recv 
xiaoming enter chat....
xiaohong enter chat....
hello xiaohong
2017/12/06 16:14:27 
        昵称 ->xiaoming:
                  hello xiaohong

2017/12/06 16:14:37 
        昵称 ->xiaohong:
                  hello xiaoming

ubuntu:~/test/1214-test/chat3.0$ ./client_chat3 192.168.65.1 xiaohong
name is :xiaohong
Socket successful!
Connect successful!
wait for thread_recv 

------------------Welecom come char------------------------

xiaohong enter chat....
2017/12/06 16:14:27 
        昵称 ->xiaoming:
                  hello xiaohong

hello xiaoming
2017/12/06 16:14:37 
        昵称 ->xiaohong:
                  hello xiaoming
				  
				  
2017/12/7 				  
是csapp.h其实就是一堆头文件的打包，在http://csapp.cs.cmu.edu/public/code.html 这里可以下载。这是《深入理解计算机系统》配套网站

Proxy源代码分析--谈谈如何学习Linux网络编程
https://www.cnblogs.com/erisen/p/6123000.html

 proxy_epoll源代码分析 linux网络编程入门的源码分析资料，附源码
http://blog.csdn.net/daniel_ustc/article/details/8687139

理解代理服务器：
从proxy.c中读出来的：
本机A  代理服务器B  服务器C
代理服务器B相当于转发的作用
代理服务器创建守护进程，B先根据用 B 自己的端口号创建a_socket_fd 服务器，这个socket负责监听本机A是否发起向B的连接。fork出
子进程，然后在子进程中创建另外一个c_socket_fd 客户端，主动向服务器C发起连接。接着用select监听这两个socket描述符，当检测到a_socket_fd可读，
则把a_socket_fd描述符读到B中的buf中，然后再把buf写到c_socket_fd描述符。当检测到c_socket_fd 描述符可读，则把c_socket_fd 读到B中的buf，然后再把
buf写到a_socket_fd描述符中去。
非常清晰！

写一个系列，从头到尾教你写一个代理服务器。
分为一、二、三、四、五篇，
先阅读开源代码，理解透，再一步一步拆解。