
//#include"http_server.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

int ServerInit(const char* ip,short port){
  int listen_sock=socket(AF_INET,SOCK_STREAM,0);
  if(listen_sock<0){
    perror("socket");
    return -1;
  }
  sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=inet_addr(ip);
  addr.sin_port=htons(port);
  int ret=bind(listen_sock,(sockaddr*)&addr,sizeof(addr));
  if(ret<0){
    perror("bind");
    return -1;
  }
  ret=listen(listen_sock,5);
  if(ret<0){
    perror("listen");
    return -1;
  }
  return listen_sock;
}

void*Threadentry(void* arg){
  int64_t new_sock=(int64_t)arg;
  //读数据并解析 此处我们无脑返回hello world 所以只是把数据读出来不解析
  char buf[1024*10]={0};
  read(new_sock,buf,sizeof(buf)-1);
  //根据请求计算响应
  //把响应写回到客户端
  const char* first_line="HTTP/1.1 200 OK\n";
  const char* blank_line="\n";
  const char* body="<h1>hello world</h1>";
  char header[1024]={0};
  sprintf(header,"Content-length: %lu\n",strlen(body));//strlen 返回值为
  //ssize_t 所以用%lu
  //把数据写回到sockt 之中
  write(new_sock,first_line,strlen(first_line));
  write(new_sock,header,strlen(header));
  write(new_sock,blank_line,strlen(blank_line));
  write(new_sock,body,strlen(body));
  close(new_sock);
  return NULL;
}


int main(int argc,char*argv[])
{
  if(argc!=3){
   printf("usage ./http_server [ip] [port]\n");
   return 1;
  }
  //////////////////////////////////////////////////////
  //HTTP基于TCP来实现的
  //1创建一个TCP服务器
  //2创建HTTP协议的格式解析请求 并且按照http格式构造响应
  int listen_sock=ServerInit(argv[1],atoi(argv[2]));
  if(listen_sock <0){
    printf("ServerInit faild\n");//为什么不是perror  没有设置错误码
    return 1;
  }
   printf("ServerInit ok\n");
   while(1){
     sockaddr_in peer;
     socklen_t len=sizeof(peer);
     int64_t new_sock=accept(listen_sock ,(sockaddr_in *)&peer,&len);
     if(new_sock<0){
        perror("accept");
        continue;
     }
     pthread_t tid;
     pthread_create(&tid,NULL,Threadentry,(void*)new_sock);
     pthread_detach(tid);

   }

  return 0;
}







