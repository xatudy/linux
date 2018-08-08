//多线程版本缓解了多进程的性能问题和资源问题，但只是多进程的一个缓解。也存在着一些缺点：相比多进程来说，不稳定，任//意一个线程挂掉，所有的线程都会退出，并且共享资源，有风险。

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

#define _port 8080

typedef struct {
   int sock;
  // char ip[24];
  // int port;
}ARG;

void service(int new_sock){
  char buf[1024]={0};
  while(1){
  size_t s=read(new_sock,buf,sizeof(buf));
   if(s<0){
     printf("read error\n");
     break;
   }else if(s==0){
     printf("read done\n");
     break;
   }else{
     buf[s]='\0';
     printf("client enter# %s\n",buf);
     //write(new_sock,buf,strlen(buf)+1);
   }
     write(new_sock,buf,strlen(buf)+1);
  }
}
void* thread_service(void*ptr){
  ARG *p=(ARG *)ptr;
  service(p->sock);
  close(p->sock);
  free(p);
  return NULL;
    
}

int main(){
  //创建套接字 
  int sock=socket(AF_INET,SOCK_STREAM,0);
  if(sock<0){
    printf("socket error\n");
    return 1;
  }

  struct sockaddr_in server_socket;
  server_socket.sin_family=AF_INET;
  server_socket.sin_port=htons(_port);
  server_socket.sin_addr.s_addr=htonl(INADDR_ANY);
  
  if(bind(sock,(struct sockaddr*)&server_socket,\
        sizeof(server_socket))<0){
    printf("bind error\n");
    return 2;
  }

  if(listen(sock,5)<0){
    printf("listen error\n");
    return 3;
  
  }
  while(1){
  struct sockaddr_in peer;
  socklen_t len =sizeof(peer);
  int new_sock=accept(sock,(struct sockaddr*)&peer,&len);
  if(new_sock<0){
    printf("accept error\n");
    return 4;
  }
  ARG *p=(ARG*)malloc(sizeof(ARG));
  if(p==NULL){
    perror("malloc");
    close(new_sock);
    continue;
  }
  p->sock=new_sock;
  //strcpy(p->ip,ipBuf);
  //p->port=p;
  pthread_t tid;
  pthread_create(&tid,NULL,thread_service,(void*)p);
  pthread_detach(tid);
  //service(new_sock);
}
  close(sock);
  return 0;
}
