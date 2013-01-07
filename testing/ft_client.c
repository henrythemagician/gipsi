//Client Program:

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
main(int argc,char**argv)
{
	int sockfd,n,connfd;
	char str[100],str1[100],s[2];
	struct sockaddr_in serv;
	if(argc!=5)
	{
		exit(0);
	}
	
	if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("\n Error ! Socket not created...\n");
		exit (0);
	}
	bzero(&serv,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(atoi(argv[2]));
	if(inet_pton(AF_INET,argv[1],&serv.sin_addr)<=0)
	{
		printf("\n error in conversion of IP address from string to num\n"),
		exit(0);
	}
	
	if((connect(sockfd,(struct sockaddr*)&serv,sizeof(serv))) < 0)
	{
		printf("\n Error!Conx not established...\n");
		exit(0);
	}
	printf("\n connected...sending file name%s\n",argv[3]);
	write(sockfd,argv[3],strlen(argv[3]));
	read(sockfd,s,3);
	write(sockfd,argv[4],strlen(argv[4]));
	str1[0]='\0';

	while((n=read(sockfd,str,100))>0)
	{
		str[n]='\0';
		printf("%s\n",str);
		write(sockfd,str,strlen(str));
	}
	if(n<0)
	printf("\n Read error...\n");
	exit (0);
}

