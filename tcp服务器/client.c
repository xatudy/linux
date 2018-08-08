

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>


#define port 8080
#define ip "127.0.0.1"

int main(){

  //创建套接字 
  int client_sock=socket(AF_INET,SOCK_STREAM,0);
  if(client_sock<0){
    perror("socket");
    return 1;
  }
  struct sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_port=htons(port);
  addr.sin_addr.s_addr=inet_addr(ip);
  //inet_addr函数将用点分十进制字符串表示的IPv4地址转化为用网络   
  //字节序整数表示的IPv4地址
  //发起连接
  if(connect(client_sock,(struct sockaddr*)&addr,\
       sizeof(addr))<0){
    printf("socket error \n");
    return 1;
  }
  char buf[1024]={0};
  while(1){
    printf("piease enter# ");
    fflush(stdout);
    ssize_t s=read(0,buf,sizeof(buf)-1);
    if(s>0){
      buf[s-1]=0;
      if(strcmp("quit",buf)==0){
        printf("client quit\n");
        break;
      }
      write(client_sock,buf,strlen(buf));
      s=read(client_sock,buf,sizeof(buf)-1);
      buf[s]=0;
      printf("server enter #%s\n",buf);
    }
  }
  close(client_sock);

  return 0;
}
