#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <stdio.h>
 
#define PORT 2080
 
main()
{
        int sock1,sock2, clength;
        sock1 =  socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in serv,cli;
 
        serv.sin_family = AF_INET;
        serv.sin_port = htons(PORT);
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");
        bind(sock1,(struct sockaddr *)&serv, sizeof(serv));
        listen(sock1,5);
        clength = sizeof(cli);
        int i=0;
        char buf[50];
     
        sock2 = accept(sock1,(struct sockaddr *)&cli,&clength);
        printf("\n Client Connected\n");
        FILE* fp = fopen("session.txt","r");
	printf("I am here outside while loop \n");
       
	 while(!feof(fp)){
                //bzero(buf,sizeof(buf));
                fread(buf,sizeof(char),50,fp);
                write(sock2,buf,50);
        }
        write(sock2,"quit1234",50);
        fclose(fp);
        return 0;
}
 
