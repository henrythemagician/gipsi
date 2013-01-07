/*
 *			GIPSI - GPAC Interactive Peer-to-peer Streaming plug-In
 *
 *  @author Haoyun Shen, Junaid Ali-Shah
 *  @version 1.0
 *
 *
 *  This file is part of GIPSI application, a p2p streaming plug-in for GPAC
 *
 *
 *
 */


#include "controlmessenger.h"
#include <stddef.h>
#include <iostream>
#include "message.h"
#include <sstream>
#include "mediaforwarder.h"
#include <cassert>
#include <fstream>



using namespace std;
#define SDPPORT 15000


extern "C"
{

void *server_SDP(void *);
int client_SDP(string ip);

}

//ControlMessenger* ControlMessenger::pCtrlMsger = NULL;

ControlMessenger::ControlMessenger(long prt)
{
    Initialize(prt);
}


ControlMessenger::~ControlMessenger()
{
   // pCtrlMsger = NULL;
   // delete pCtrlMsger;
}

void ControlMessenger::Initialize(long prt)
{
    cout<<"Initializing Control Messenger at port: "<<prt<<"....."<<endl;
    ifFirstPeer=true;

}

void ControlMessenger::setPort(long prt)
{
    port = prt;
}

long ControlMessenger::getPort()
{
    return port;
}

bool ControlMessenger::getIfSrc()
{
    return ifSource;
}



void ControlMessenger::MessageListener()
{
     cout<<"Accessing MessageListener of singleton instance of ControlMessenger...."<<endl;
    long sockfd,n;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t len;
    char mesg[1000];

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(40000);
    bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    while(1)
    {
        len = sizeof(cliaddr);
        cout<<"Entering the while loop for receiving message...."<<endl;
        n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
        // sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
        printf("-------------------------------------------------------\n");
        //	sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
        mesg[n] = 0;
        printf("Received the following:\n");


        //to get the ip address of the client
        string cIP=inet_ntoa(cliaddr.sin_addr);
        cout<<"this is the IP address of the client "<<cIP<< endl;


        char *ch = strtok(mesg, " ");
        string ip = ch;
      //  cout<<"this is the first token " <<ip <<endl;
        ch = strtok(NULL, " ,");
        long port= atol(ch);
      //  cout<<"this is the second token " << port <<endl;
        ch = strtok(NULL, " ,");
        long type= atol(ch);
      //  cout<<"this is the third token " << type <<endl;
        ch = strtok(NULL, " ,");


        switch (type)
        {
        case 1://request of streaming from peer to source
        {
            cout<<"Type "<< type <<" message received from " << cIP<< " ....." <<endl;
            long tp=2;

            /*//////////////////////////////////////////////////////////////////////
                    pthread_t server_sdp_thread;
                   int rc;
               rc = pthread_create(&server_sdp_thread, NULL, server_SDP,(void *)NULL);
                    if (rc){
                      printf("ERROR; return code from pthread_create() is %d\n", rc);
                       exit(-1);
                     }

            //////////////////////////////////////////////////////////////////////////*/
            //cout<<"FilePath:" << this->getFilePath()<< " ....." <<endl;

            if (cIP!="127.0.0.1" || port!=30000)
            {
               // MediaForwarder::Inst()->addAudience(cIP,port);
                mFwd->addAudience(cIP,port);
               // MediaForwarder::Inst()->startStreaming(MediaForwarder::Inst()->getFilePath(),cIP,port);
            }

          this->sendMessage(cIP,port,tp);


             pthread_exit(NULL);

      }
            break;
        case 2://reply of streaming from source to peer
         {
            cout<<"Type "<< type <<" message received from " << cIP<< " ....." <<endl;

            /////////////////////////////////////////////////////////////
            int r = client_SDP(cIP);
            ///////////////////////////////////////////////////

            string str_port;
            stringstream stream1;
            stream1<< port;
            stream1>> str_port;
            mFwd->addAudience("127.0.0.1",7000);

            string cmdMP4="MP4Client session.sdp";
         //    string cmdMP4="MP4Client sessions/"+ip+".sdp";
            system(cmdMP4.c_str());
        }
            break;
        default :// unrecognized message received
            break;
        }

        printf("-------------------------------------------------------\n");
    }

}

void ControlMessenger::sendMessage(string ip,long prt, long tp)
{

    int r;
     cout<<"Accessing sendMessage of singleton instance of ControlMessenger...."<<endl;
    Message *msg = new Message(ip,prt,tp);


    int sock;
    struct sockaddr_in server_addr;
    struct hostent *host;
    string send_data;
    string str_sdp;
    //define the IP address of the source

    host= (struct hostent *) gethostbyname(ip.c_str());
    assert(host);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
    perror("socket");
    exit(1);
    }

    server_addr.sin_family = AF_INET;

    //define the control messaging port of the server
    server_addr.sin_port = htons(40000);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);

    bzero(&(server_addr.sin_zero),8);

    send_data=msg->toString()+" "+str_sdp;


    const char *p = send_data.c_str();

    cout<<"The data **"<<p<<"** is sent from client............"<<endl;

    sendto(sock, p, strlen(p), 0,
       (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    msg= NULL;
    delete msg;


}

void ControlMessenger::setFilePath(string fp)
{
    filePath=fp;
}
string ControlMessenger::getFilePath()
{
    return filePath;
}

extern "C"
{

void *server_SDP(void *args)
{
    int sock1,sock2, clength;
           sock1 =  socket(AF_INET,SOCK_STREAM,0);
           struct sockaddr_in serv,cli;

           serv.sin_family = AF_INET;
           serv.sin_port = htons(2080);
           serv.sin_addr.s_addr = inet_addr("127.0.0.1");
           bind(sock1,(struct sockaddr *)&serv, sizeof(serv));
           listen(sock1,5);
           clength = sizeof(cli);
           int i=0;
           char buf[50];
           sock2 = accept(sock1,(struct sockaddr *)&cli,(socklen_t*)&clength);
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

}

int client_SDP(string ip)
{
    int 