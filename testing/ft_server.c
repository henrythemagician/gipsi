//Server Program:

#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<time.h>
#define MAX 100;
int main ()
{
	int listenfd,connfd,n,n1;
	struct sockaddr_in serv;
	char str1 [100],str2[100],fname[20],fname1[20],s[2];
	int port=9999;
	FILE*f1,*f2;
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&serv,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_addr.s_addr=htonl(INADDR_ANY);
	serv.sin_port=htons(port);
	bind(listenfd,(struct sockaddr*)&serv,sizeof(serv));
	//"serftp4.cpp"54L, 1518C
	listen(listenfd,5);
	for(;;)
	{
		connfd=accept(listenfd,(struct sockaddr*)NULL,NULL);
		printf("\nClient requesting");
		n=read(connfd,fname,20);
		fname[n]='\0';
		printf("\n Received:%s\n",fname);
		f1=fopen(fname,"r");
		strcpy(s,"ab");
		write(connfd,s,strlen(s));
		n1=read(connfd,fname1,20);
		fname1[n1]='\0';
		printf("stored in:%s\n",fname1);
		f2=fopen(fname1,"w");
		while(!feof(f1))
		{
			fgets(str1,50,f1);
			write(connfd,str1,strlen(str1));
			n=read(connfd,str2,100);
			str2[n]='\0';
			fputs(str2,f2);
		}
		fclose(f1);
		fclose(f2);
		close(connfd);
	}
	return 0;
}


