/*
 ******************************************************************************
 * Copyright (C) Haoyun Shen, Junaid Ali-Shah 2011
 *
 * This file is part of GIPSI - GPAC Interactive Peer-to-peer Streaming plug-In.
 *
 *    GIPSI is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    GIPSI is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with GIPSI.  If not, see <http://www.gnu.org/licenses/>
 ******************************************************************************
 */

#include "MediaForwarder.h"
#include "Optimizer.h"
#include "ControlMessenger.h"
using namespace std;
void *forwarding(void *args);

MediaForwarder* MediaForwarder::pMdFrwder = NULL;

MediaForwarder::MediaForwarder()
{
}

MediaForwarder* MediaForwarder::Inst()
{
   if(pMdFrwder == NULL)
    {
      pMdFrwder = new MediaForwarder();
    }
   assert(pMdFrwder);
   return pMdFrwder;
}

MediaForwarder::~MediaForwarder()
{
    pMdFrwder = NULL;
    delete pMdFrwder;
}


void MediaForwarder::startStreaming(string fp)
{


    string cmdMP4="MP4Box -rtp "+fp+"";

    system(cmdMP4.c_str());


}

void MediaForwarder::addChildAsForwarder(string ip,int prt)
{
    //cout<<"Access addChildAsForwarder for ip: "<<ip<<" port: "<<prt<<endl;

    for(int i=0;i<2;i++)
    {

        for (list<struct ForwardSock>::iterator item = fwdSocks.begin(); item != fwdSocks.end(); item++)
       {

            {
                for (list<struct Sock>::iterator it = (*item).dests.begin(); it != (*item).dests.end(); it++)
               {

                    if((*it).port==7000+(2*i)){

                    struct Sock newDest;
                    newDest.ip=ip;
                    newDest.port=prt+(2*i);
                    int sock2=socket(AF_INET, SOCK_DGRAM, 0);
                    newDest.socket=sock2;
                    (*item).dests.push_back(newDest);
                    cout<<"Adding destination "<<newDest.ip<<":"<<newDest.port<<" for source port"<<(*item).srcPort<<endl;
                    }
               }
            }



        }

    }
    this->printFwdSocks();

}
void MediaForwarder::addChild(string ip,int prt)
{
    //cout<<"Access addChild"<<endl;

    for(int i=0;i<2;i++)
    {
        if (!checkSrcPort(7000+(2*i)))
        {
            struct Sock newDest;
            newDest.ip=ip;
            newDest.port=prt+(2*i);
            int sock2=socket(AF_INET, SOCK_DGRAM, 0);
            newDest.socket=sock2;
            struct ForwardSock newFwdSock;
            newFwdSock.srcPort=7000+(2*i);
            newFwdSock.dests.push_back(newDest);
            fwdSocks.push_back(newFwdSock);
            pthread_t threadFwd;
            int rc;
            int *prtForNewThread=new int(7000+(2*i));
            rc = pthread_create(&threadFwd, NULL, forwarding, (void *)prtForNewThread);
            if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
            }

        }
        else
        {

            for (list<struct ForwardSock>::iterator it = fwdSocks.begin(); it != fwdSocks.end(); it++)
           {
                if ((*it).srcPort==(7000+(2*i)))
                {
                    struct Sock newDest;
                    newDest.ip=ip;
                    newDest.port=prt+(2*i);
                    int sock2=socket(AF_INET, SOCK_DGRAM, 0);
                    newDest.socket=sock2;
                    (*it).dests.push_back(newDest);
                    cout<<"Adding destination "<<newDest.ip<<":"<<newDest.port<<" for source port"<<7000+(2*i)<<endl;
                }

            }

        }

    }
    this->printFwdSocks();

}

void MediaForwarder::addAudience(string ip,int prt)
{

    //cout<<"Access addAudience"<<endl;


    for(int i=1;i<3;i++)
    {
        if (!checkSrcPort(7000+(2*i-1)))
        {
            struct Sock newDest;
            newDest.ip=ip;
            newDest.port=prt+(2*i-1);
            int sock2=socket(AF_INET, SOCK_DGRAM, 0);
            newDest.socket=sock2;
            struct ForwardSock newFwdSock;
            newFwdSock.srcPort=7000+(2*i-1);
            newFwdSock.dests.push_back(newDest);
            fwdSocks.push_back(newFwdSock);
            pthread_t threadFwd;
            int rc;
            int *prtForNewThread=new int(7000+(2*i-1));
            rc = pthread_create(&threadFwd, NULL, forwarding, (void *)prtForNewThread);
            if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
            }
        }
        else
        {
            bool ifIPFound=false;

             for (list<struct ForwardSock>::iterator item = fwdSocks.begin(); item != fwdSocks.end(); item++)
           {

                 for (list<struct Sock>::iterator it = (*item).dests.begin(); it != (*item).dests.end(); it++)
                {
                     if (ip==(*it).ip && (*it).port== prt+(2*i-1))
                     {
                         cout<<"Source Port: "<<(*item).srcPort<<" forwarding to:"<<(*it).ip<<":"<<(*it).port<<" already exist........"<<endl;
                         ifIPFound=true;
                     }
                }
                 if ((*item).srcPort==(7000+(2*i-1))&&!ifIPFound)
                {

                        struct Sock newDest;
                        newDest.ip=ip;
                        newDest.port=prt+(2*i-1);
                        int sock2=socket(AF_INET, SOCK_DGRAM, 0);
                        newDest.socket=sock2;
                        (*item).dests.push_back(newDest);
                        cout<<"Adding destination "<<newDest.ip<<":"<<newDest.port<<" for source port:"<<7000+(2*i-1)<<endl;


               }
            }

        }
        if(!checkSrcPort(prt+(2*i-1))){
            struct Sock newDest;
            newDest.ip="127.0.0.1";
            newDest.port=7000+(2*i-1);
            int sock2=socket(AF_INET, SOCK_DGRAM, 0);
            newDest.socket=sock2;
            struct ForwardSock newFwdSock;
            newFwdSock.srcPort=prt+(2*i-1);
            newFwdSock.dests.push_back(newDest);
            fwdSocks.push_back(newFwdSock);
            pthread_t threadFwd;
            int rc;
            int *prtForNewThread=new int(prt+(2*i-1));
            rc = pthread_create(&threadFwd, NULL, forwarding, (void *)prtForNewThread);
            if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
            }
        }
    }

    cout<<"AddAudience "<<ip<<" at MediaForwarder..."<<endl;
    this->printFwdSocks();


}


void MediaForwarder::removeDestination(string ip)
{
    cout<<"Remove destination"<<ip<<endl;

     for (list<struct ForwardSock>::iterator item = fwdSocks.begin(); item != fwdSocks.end(); item++)
       {
            {
            bool destFound=false;
            for (list<struct Sock>::iterator it = (*item).dests.begin(); it != (*item).dests.end(); it++)
           {

                if(ip==(*it).ip){

                    destFound=true;

                    close((*it).socket);
                    cout<<"Removing Destination "<<(*it).ip<<":"<<(*it).port<<" for source port"<<(*item).srcPort<<"........"<<endl;
                    it=(*item).dests.erase(it);
                }
           }
            if(!destFound)
            {
                cout<<"Removing Destination "<<ip<<" for source port"<<(*item).srcPort<<" not found"<<endl;
            }
            }
        }

    this->printFwdSocks();

}
void MediaForwarder::signalingFrom(string ip,int prt)
{
    //cout<<"Signaling From"<<endl;
    for(int i=1;i<3;i++)
     {
         {
             struct Sock newDest;
             newDest.ip="127.0.0.1";
             newDest.port=7000+(2*i-1);
             int sock2=socket(AF_INET, SOCK_DGRAM, 0);
             newDest.socket=sock2;
             struct ForwardSock newFwdSock;
             newFwdSock.srcPort=prt+(2*i-1);
             newFwdSock.dests.push_back(newDest);
             fwdSocks.push_back(newFwdSock);
             pthread_t threadFwd;
             int rc;
             int *prtForNewThread=new int(prt+(2*i-1));
             rc = pthread_create(&threadFwd, NULL, forwarding, (void *)prtForNewThread);
             if (rc){
             printf("ERROR; return code from pthread_create() is %d\n", rc);
             exit(-1);
             }



         }
       {
             struct Sock newDest;
             newDest.ip=ip;
             newDest.port=prt+(2*i-1);
             int sock2=socket(AF_INET, SOCK_DGRAM, 0);
             newDest.socket=sock2;
             struct ForwardSock newFwdSock;
             newFwdSock.srcPort=7000+(2*i-1);
             newFwdSock.dests.push_back(newDest);
             fwdSocks.push_back(newFwdSock);
             pthread_t threadFwd;
             int rc;
             int *prtForNewThread=new int(7000+(2*i-1));
             rc = pthread_create(&threadFwd, NULL, forwarding, (void *)prtForNewThread);
             if (rc){
             printf("ERROR; return code from pthread_create() is %d\n", rc);
             exit(-1);
             }
         }
     }
    this->printFwdSocks();

}
void MediaForwarder::receiveFrom(int prt)
{

   // cout<<"Receive from"<<endl;

   for(int i=0;i<2;i++)
    {
        if (!checkSrcPort(prt+(2*i)))
        {
            struct Sock newDest;
            newDest.ip="127.0.0.1";
            newDest.port=7000+(2*i);
            int sock2=socket(AF_INET, SOCK_DGRAM, 0);
            newDest.socket=sock2;
            struct ForwardSock newFwdSock;
            newFwdSock.srcPort=prt+(2*i);
            newFwdSock.dests.push_back(newDest);
            fwdSocks.push_back(newFwdSock);
            pthread_t threadFwd;
            int rc;
            int *prtForNewThread=new int(prt+(2*i));
            rc = pthread_create(&threadFwd, NULL, forwarding, (void *)prtForNewThread);
            if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
            }
        }
        else
        {

             for (list<struct ForwardSock>::iterator it = fwdSocks.begin(); it != fwdSocks.end(); it++)
           {
               // struct ForwardSock item=*it;
                if ((*it).srcPort==(prt+(2*i)))
                {
                    struct Sock newDest;

                    newDest.ip="127.0.0.1";
                    newDest.port=7000+(2*i);
                    int sock2=socket(AF_INET, SOCK_DGRAM, 0);
                    newDest.socket=sock2;
                    list <struct Sock>::iterator itDests;
                    itDests=(*it).dests.begin();
                    itDests=(*it).dests.erase(itDests);
                    (*it).dests.push_back(newDest);
                    cout<<"Adding destination "<<newDest.ip<<":"<<newDest.port<<" for source port"<<prt+(2*i)<<endl;
                }
            }
        }

    }

   // cout<<"Receive From "<<prt<<" at MediaForwarder..."<<endl;
    this->printFwdSocks();

}

void MediaForwarder::setMediaPort(int mPrt)
{
    mediaPort=mPrt;
}

void MediaForwarder::setFilePath(string fp)
{
    filePath=fp;
}

int MediaForwarder::getMediaPort()
{
    return mediaPort;
}

string MediaForwarder::getFilePath()
{
    return filePath;
}

void *forwarding(void *args)
{

    int* threadPort=(int*)args;
    cout<<"@forwarding"<<*threadPort<<endl;
    MediaForwarder::Inst()->forwardMedia(*threadPort);
    delete threadPort;
    threadPort=NULL;
    return NULL;
}

void MediaForwarder::printFwdSocks()
{
    cout<<"################################################################"<<endl;

    for (list<struct ForwardSock>::iterator item = fwdSocks.begin(); item != fwdSocks.end(); item++)
   {

        for (list<struct Sock>::iterator it = (*item).dests.begin(); it != (*item).dests.end(); it++)
       {


                cout<<"Source Port: "<<(*item).srcPort<<" forwarding to:"<<(*it).ip<<":"<<(*it).port<<"........"<<endl;

       }

    }
    cout<<"################################################################"<<endl;
}

void MediaForwarder::forwardMedia(int srcPort){
      int sock, length, n,on;
      socklen_t fromlen;
      struct sockaddr_in server;
      struct sockaddr_in from;
      char buf[MEDIAMTU];//what about this size?
      printf("Redirecting from port: %d \n",srcPort);


      sock=socket(AF_INET, SOCK_DGRAM, 0);//AF_INET means ipv4, SOCK_DGRAM means udp datagram
      if (sock < 0) perror("Opening socket");

      length = sizeof(server);

      bzero(&server,length);//initializing the "server" by setting its content to 0

      server.sin_family=AF_INET;//specifying the server address, with any local address
      server.sin_addr.s_addr=INADDR_ANY;
      server.sin_port=htons(srcPort);
       on=1;
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));


      if (bind(sock,(struct sockaddr *)&server,length)<0)
          perror("binding");

      fromlen = sizeof(struct sockaddr_in);
      unsigned int length2;
      struct sockaddr_in receiver;
      receiver.sin_family = AF_INET;
      length2=sizeof(struct sockaddr_in);
      int n2;
      string inputIP;
     // int sock2;
      struct ForwardSock item;

      while (1) {
          n = recvfrom(sock,buf,MEDIAMTU,0,(struct sockaddr *)&from,&fromlen);
          inputIP=inet_ntoa(from.sin_addr);
          for (list<struct ForwardSock>::iterator it = fwdSocks.begin(); it != fwdSocks.end(); it++)
         {
              item=*it;
              if (item.srcPort==srcPort)
              {

                  for (list<struct Sock>::iterator it = item.dests.begin(); it != item.dests.end(); it++)
                 {

                      //sock2= socket(AF_INET, SOCK_DGRAM, 0);
                      receiver.sin_addr.s_addr= inet_addr((*it).ip.c_str());
                      receiver.sin_port = htons((*it).port);
                      n2=sendto((*it).socket,buf, MEDIAMTU,MSG_DONTWAIT,(const struct sockaddr *)&receiver,length2);
                      if ((*it).ip=="127.0.0.1" && Optimizer::Inst()->getParent()!=inputIP && !ControlMessenger::Inst()->getIfSrc() && ((*it).port==7000||(*it).port==7002))
                      {


                          Optimizer::Inst()->setParent(inputIP);
                          if (Optimizer::Inst()->treeBalancing()!="1")
                          {
                              string treeBalance=Optimizer::Inst()->treeBalancing();
                              cout<<"Tree banlancing result is:"<<treeBalance<<"..."<<endl;
                              ControlMessenger::Inst()->sendMessage(treeBalance,0,TREEBALANCE,ControlMessenger::Inst()->getSelfIP());
                          }

                      }
                      //close(sock2);

                 }
              }
          }

      }
      close(sock);
      exit(0);
}

bool MediaForwarder::checkSrcPort(int port)
{

    for (list<struct ForwardSock>::iterator it = fwdSocks.begin(); it != fwdSocks.end(); it++)
   {
        struct ForwardSock item=*it;
        if (item.srcPort==port)
        {
            return true;
        }
    }
    return false;
}
