
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