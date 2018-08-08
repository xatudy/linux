//多进程服务器
//
//缺点：
//1.只有链接来了才创建进程，影响性能。（类似于我们生活中的例子，到了饭店了老板才开始买菜），解决这一问题，可以提//前创建，进程池的引入。 
//2.能够同时服务量有限，因为每个进程都需要分配资源。 
//3.进程增多导致切换增多，调度周期增长，进而影响性能。
//
//优点：
//1.可以同时服务多个客户。 
//2.易于编写。 
//3.稳定，任意一个进程挂掉，不会影响其他进程。


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/un.h> 
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<netinet/in.h>

#define _port 8080
void service(int accept_fd){
    char buf[1024]={0};
    while(1){
     ssize_t s=read(accept_fd,buf,sizeof(buf)-1);
     if(s>0){
     buf[s]=0;
     printf("say#%s\n",buf);
     write(accept_fd,buf,strlen(buf));
    } 
    else if(s==0)
    {
       printf("read done\n");
       break;
     }else{
     perror("read");
     break;
     }
   }
}
void createfork(int accept_fd,int listen_sock){

  pid_t pid=fork();
  if(pid==0){//child
    close(listen_sock);
      if(fork()==0){     
    //孙子进程为孤儿进程，系统回收
      service(accept_fd);
     }
    exit(0);
  }else if(pid>0){//father
    close(accept_fd);
    waitpid(pid,NULL,0);//父进程回收子进程
  }else{
    printf("fork error\n");
    exit(1);
  }
}



int main()
{
  //  创建套接字 用于监听
  int sock=socket(AF_INET,SOCK_STREAM,0);
  if(sock<0){

    printf("socket error errno is:%d\n",errno);
  }
  struct sockaddr_in server_socket;
  server_socket.sin_family=AF_INET;
  //htonl就是把32位本机字节顺序转化为网络字节顺序
  server_socket.sin_addr.s_addr=htonl(INADDR_ANY);
 //本函数将一个16位数从主机字节顺序转换成网络字节顺序。
 //返回值：
 //htons()返回一个网络字节顺序的值。
  server_socket.sin_port=htons(_port);
  
  int on=1;
  int s=setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  if(s<0){
    perror("setsockopt");
    exit(1);
  }
  //调用bind函数，将该套接字绑定到一个地址，并制定一个端口号
  if(bind(sock,(struct sockaddr*)&server_socket,\
        sizeof(server_socket))<0)
  {
    printf("bind error errno is:%d\n",errno);
    close(sock);
    return 1;
  }
//调用listen函数，使用该套接字监听连接请求
  int listen_sock=listen(sock,5);
  if(listen_sock<0){
   printf("listen error\n");
   close(sock);
   return 2;
  }
  printf("bind and listen success ,wait accept....\n");
      //接受连接  
      //系统调用从listen监听队列中接受一个连接  
      //int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen)  
      //sockfd参数是执行过listen系统调用的监听socket；addr参数用来获取被  
      //接受连接的远端socket地址，该socket地址的长度由addrlen参数指出  
      //accept成功时返回一个新的连接socket，该socket唯一的标识了被接受  
      //的这个连接，服务器可通过读写该socket来与被接受连接对应的客户端通信 
while(1){
  struct sockaddr_in client_socket;
  socklen_t len=0;
 // char buf[1024]={0};
  int accept_fd=accept(sock,(struct sockaddr*)&client_socket,\
      &len);
  if(accept_fd<0)
  { 
     printf("accept error,errno is:%d\n",errno);
     close(sock);
     return 3;
  }
  //memset(buf,'\0',sizeof(buf));
  // printf("connect ip:%s,port:%d\n",inet_ntop(AF_INET,
       // &client_socket.sin_addr,buf,sizeof(buf)),
    //  ntohs(client_socket.sin_port));
 // while(1){
  
   // memset(buf,'\0',sizeof(buf));
   // ssize_t size=read(accept_fd,buf,sizeof(buf)-1);
   // if(size<0){
     // printf("read error\n");
     // break;
   // }else if(size==0){

     // printf("read done\n");
     // break;
   // }else{

     // printf("client# %s\n",buf);
   // }
   // printf("server enter# ");
   // fflush(stdout);
   // size=read(0,buf,sizeof(buf)-1);
   // if(size>0){
     // buf[size-1]='\0';

   // }else if(size==0){
     // printf("read done...\n");
     // break;

   // }else{
     // perror("read");
     // break;

   // }
   // write(accept_fd,buf,strlen(buf));
   //printf("please wait...\n");
    // char buf[1024]={0};
 // }
  createfork(accept_fd,listen_sock);
  close(sock);
}
  return 0;
}
