#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <stdio.h>
 
#define PORT 2080
 
main()
{
        int sock1;
        sock1 =  socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in serv;
 
        serv.sin_port = htons(PORT);
        printf("%x %x\n",PORT,htons(PORT));
        serv.sin_family = AF_INET;
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");
        printf("client connecting\n");
        connect(sock1, (struct sockaddr *)&serv,sizeof(serv));
        char buf[50];
        FILE* fp = fopen("test.txt","w");
        while(1){
                //bzero(buf,sizeof(buf));
                read(sock1,buf,50);
                if(strcmp(buf,"quit1234")==0)
                {
                        break;  
                }
                fprintf(fp,"%s",buf);
        }
        fclose(fp);
}
