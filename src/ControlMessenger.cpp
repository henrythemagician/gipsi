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

#include "ControlMessenger.h"
#include "Message.h"
#include "Optimizer.h"

using namespace std;


void *media_Forwarder(void *args);
void *send_sdp(void *);
void receive_sdp(string );
void *play(void *args);
void *goodbye(void *args);
void *keepalive(void *args);
void *checkalive(void *args);

ControlMessenger::ControlMessenger(int prt)
{
    Initialize(prt);
}
 //Code for Singleton
ControlMessenger* ControlMessenger::pCtrlMsger = NULL;

ControlMessenger* ControlMessenger::Inst()
{
    //cout<<"Access singleton instance of Control Messenger at port:"<<SIGNALING_PORT<<"....."<<endl;
    if(pCtrlMsger == NULL)
    {
        pCtrlMsger = new ControlMessenger(EnvironmentalSettings::Inst()->SIGNALING_PORT);
    }
    return pCtrlMsger;
}

ControlMessenger::~ControlMessenger()
{
    pCtrlMsger = NULL;
    delete pCtrlMsger;
}

void ControlMessenger::Initialize(int prt)
{
  // cout<<"Initializing Control Messenger at port: "<<prt<<"....."<<endl;
    ifFirstPeer=true;
    signalingPort=prt;
    ifCheckAliveEstablished=false;
    ifKeepAliveEstablished=false;
    keepAliveList.clear();
    ifSource=false;

}

void ControlMessenger::setMediaPort(int prt)
{
    mediaPort=prt;
}
int ControlMessenger::getMediaPort()
{
    return mediaPort;
}
void ControlMessenger::setPort(int prt)
{
    signalingPort = prt;
}

int ControlMessenger::getPort()
{
    return signalingPort;
}

bool ControlMessenger::getIfSrc()
{
    return ifSource;
}
void ControlMessenger::setIfSrc(bool IF)
{
    ifSource=IF;
}
void ControlMessenger::setSrcAddr(string addr)
{
    srcAddr=addr;
}

string ControlMessenger::getSrcAddr()
{
    return srcAddr;
}

void ControlMessenger::messageListener()
{
    //  cout<<"Accessing MessageListener of singleton instance of ControlMessenger...."<<endl;

    if(ifSource)
    {
        MediaForwarder::Inst()->setFilePath(filePath);
        pthread_t threadMF;
        int rc2;
        rc2 = pthread_create(&threadMF, NULL, media_Forwarder,(void *)MediaForwarder::Inst());
        if (rc2){
        printf("ERROR; return code from pthread_create() is %d\n", rc2);
        exit(-1);
        }

    }
    int sockfd,n;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t len;
    char mesg[1000];

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(EnvironmentalSettings::Inst()->SIGNALING_PORT);
    bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    string cIP;
    while(1)
    {
        len = sizeof(cliaddr);
        //cout<<"-------------------------------------------------------\n";
        //cout<<"Entering the while loop for receiving message...."<<endl;

        n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);

        mesg[n] = 0;

        //to get the ip address of the client
        cIP=inet_ntoa(cliaddr.sin_addr);

        //Interpreting the received message
        char *ch = strtok(mesg, " ");
        string receiverIP=ch;
        ch = strtok(NULL, " ,");
        int port= atol(ch);
        ch = strtok(NULL, " ,");
        int type= atol(ch);
        ch = strtok(NULL, " ,");
        string messageReceived=ch;
        ch = strtok(NULL, " ,");

        selfIP=receiverIP;
        switch (type)
        {
            case CONNECT://CONNECT message received
            {
                ////////////////////////////////////////////////////////////////////////////////////////////////////
                if(!EnvironmentalSettings::Inst()->LABMODE)
                {
                    pthread_t send_sdp_thread;
                    int rc;
                    rc = pthread_create(&send_sdp_thread, NULL, send_sdp, (void *)NULL);
                    if (rc){
                      printf("ERROR; return code from pthread_create() is %d\n", rc);
                       exit(-1);
                     }
                }
                ////////////////////////////////////////////////////////////////////////////////////////////////////////

                cout<<"@"<<receiverIP<<": CONNECT message: <"<<messageReceived<<"> received from " << cIP<< " ....." <<endl;

                if(ifFirstPeer)
                {
                    string debugMsg="addSource 0-0 "+receiverIP+"";
                    this->sendDebugMessage(debugMsg);
                }
                {
                    string debugMsg="addNode 0-0 "+cIP+"";
                    this->sendDebugMessage(debugMsg);
                }

                // Optimizer::Inst()->startStreaming();
                string addChildTo=Optimizer::Inst()->addChild(receiverIP,cIP,port);
                MediaForwarder::Inst()->addAudience(cIP,port);
                if (addChildTo=="1"){//add the child locally, tell this lucky guy the good news

                    ifFirstPeer=false;
                    //int replyType=ACCEPT;
                    //this->sendMessage(cIP,port,replyType,addChildTo);

                    MediaForwarder::Inst()->addChild(cIP,port);
                    string debugMsg;

                    debugMsg="addEdge 0-0 "+this->getSelfIP()+" "+cIP+"";
                    this->sendDebugMessage(debugMsg);

                    this->sendMessage(cIP,0,KEEPALIVE,"KEEPALIVE");

                }else{//no available uploadcapacity locally, ask the loneliest child
                    int replyType=DROP;

                    this->sendMessage(addChildTo,port,replyType,cIP);
                    Optimizer::Inst()->childAddedAt(addChildTo);
                }
                Optimizer::Inst()->printTopoInfo();
            }
                break;
            case DROP://DROP message received
            {
                cout<<"@"<<receiverIP<<": DROP message: <"<<messageReceived<<"> received from " << cIP<< " ....." <<endl;

                size_t pos;
                pos = messageReceived.find("_");
                if (pos== string::npos)
                {
                    string addChildTo=Optimizer::Inst()->addChild(cIP,messageReceived,port);
                    if (addChildTo=="1"){//add child locally
                        //int replyType=ACCEPT;
                        //this->sendMessage(messageReceived,port,replyType,addChildTo);
                        if(!ifSource)
                        {
                           // this->sendMessage(Optimizer::Inst()->getParent(),port,DROP_REPLY,"one");//inform parent to add numberofchild
                            MediaForwarder::Inst()->addChildAsForwarder(messageReceived,port);
                            this->sendMessage(messageReceived,0,KEEPALIVE,"KEEPALIVE");
                            string debugMsg="addEdge 0-0 "+this->getSelfIP()+" "+messageReceived+"";
                            this->sendDebugMessage(debugMsg);
                        }
                        else
                        {
                            MediaForwarder::Inst()->addChild(messageReceived,port);
                            string debugMsg="addEdge 0-0 "+this->getSelfIP()+" "+messageReceived+"";
                            this->sendDebugMessage(debugMsg);
                             this->sendMessage(messageReceived,0,KEEPALIVE,"KEEPALIVE");
                        }
                    }else{

                        this->sendMessage(addChildTo,port,DROP,messageReceived);
                        Optimizer::Inst()->childAddedAt(addChildTo);

                    }
                }
                else
                {
                    int numOfChild = atoi(messageReceived.substr (pos+1).c_str());
                    string droppedAddr = messageReceived.substr(0,pos);
                    cout<<droppedAddr<<" "<<numOfChild<<endl;
                    cout<<messageReceived<<endl;

                    string addChildTo=Optimizer::Inst()->addChild(cIP,droppedAddr,port);
                    if (addChildTo=="1"){//add child locally
                        //int replyType=ACCEPT;
                        //this->sendMessage(droppedAddr,port,replyType,addChildTo);
                        if(!ifSource)
                        {
                            stringstream stream1;
                            string str_numOfChild;
                            stream1<< numOfChild+1;
                            stream1>> str_numOfChild;

                           // this->sendMessage(Optimizer::Inst()->getParent(),port,DROP_REPLY,str_numOfChild);//inform parent to add numberofchild
                            MediaForwarder::Inst()->addChildAsForwarder(droppedAddr,port);
                            this->sendMessage(droppedAddr,0,KEEPALIVE,"KEEPALIVE");
                            string debugMsg;
                            debugMsg="addEdge 0-0 "+this->getSelfIP()+" "+droppedAddr+"";
                            this->sendDebugMessage(debugMsg);
                            for (int i=0;i<numOfChild;i++)
                            {
                               // cout<<"adding number of child of"<<droppedAddr<<"to "<<i<<endl;
                                Optimizer::Inst()->childAddedAt(droppedAddr);
                            }

                        }
                        else
                        {
                            MediaForwarder::Inst()->addChild(droppedAddr,port);
                            for (int i=0;i<numOfChild;i++)
                            {
                               // cout<<"adding number of child of"<<droppedAddr<<"to "<<i<<endl;
                                Optimizer::Inst()->childAddedAt(droppedAddr);
                            }

                        }
                        if (Optimizer::Inst()->treeBalancing()!="1")
                        {
                            cout<<"Tree banlancing result is:"<<Optimizer::Inst()->treeBalancing()<<"..."<<endl;
                            this->sendMessage(Optimizer::Inst()->treeBalancing(),0,TREEBALANCE,receiverIP);
                        }
                        else
                        {
                            cout<<"Tree is balanced already, no need to repair.."<<endl;
                        }
                    }else{

                        this->sendMessage(addChildTo,port,DROP,messageReceived);
                        Optimizer::Inst()->childAddedAt(addChildTo);
                        for (int i=0;i<numOfChild;i++)
                        {
                           // cout<<"adding number of child of"<<addChildTo<<"to "<<i<<endl;
                            Optimizer::Inst()->childAddedAt(addChildTo);
                        }
                        if (Optimizer::Inst()->treeBalancing()!="1")
                        {
                            cout<<"Tree banlancing result is:"<<Optimizer::Inst()->treeBalancing()<<"..."<<endl;
                            this->sendMessage(Optimizer::Inst()->treeBalancing(),0,TREEBALANCE,receiverIP);
                        }
                        else
                        {
                            cout<<"Tree is balanced already, no need to repair.."<<endl;
                        }
                    }
                }

                Optimizer::Inst()->printTopoInfo();
            }
                break;
           /* case ACCEPT://ACCEPT message received from loneliest parent
           {
               cout<<"@"<<receiverIP<<" ACCEPT message received from " << cIP<<":" << port<<" ....." <<endl;


               if( Optimizer::Inst()->getParent()=="")
               {
                   Optimizer::Inst()->startStreaming(cIP);
                   MediaForwarder::Inst()->signalingFrom(srcAddr,mediaPort);
                   MediaForwarder::Inst()->receiveFrom(cIP,port);
                   {
                       string debugMsg="addEdge 0-0 "+cIP+" "+receiverIP+"";
                       this->sendDebugMessage(debugMsg);
                   }
                   pthread_t threadPLAY;
                   int rc2;
                   rc2 = pthread_create(&threadPLAY, NULL, play,(void*)NULL);
                   if (rc2){
                   printf("ERROR; return code from pthread_create() is %d\n", rc2);
                   exit(-1);
                   }*/
                   /*pthread_t threadGoodbye;
                   rc2 = pthread_create(&threadGoodbye, NULL, goodbye,(void*)&threadPLAY);
                   if (rc2){
                   printf("ERROR; return code from pthread_create() is %d\n", rc2);
                   exit(-1);
                   }*/
                   /*if (!ifKeepAliveEstablished)
                   {
                       pthread_t keepAlive;
                       rc2 = pthread_create(&keepAlive, NULL, keepalive,(void*)NULL);
                       if (rc2){
                       printf("ERROR; return code from pthread_create() is %d\n", rc2);
                       exit(-1);
                       }
                       ifKeepAliveEstablished=true;
                   }
               }
               else
               {

                   {
                       string debugMsg="removeEdge 0-0 "+Optimizer::Inst()->getParent()+" "+receiverIP+"";
                       this->sendDebugMessage(debugMsg);
                   }
                   {
                       string debugMsg="addEdge 0-0 "+cIP+" "+receiverIP+"";
                       this->sendDebugMessage(debugMsg);
                   }

                   Optimizer::Inst()->startStreaming(cIP);

               }
               Optimizer::Inst()->printTopoInfo();
           }
               break;*/
            case DISCONNECT://DISCONNECT message received from a leaf child
            {
                cout<<"@"<<receiverIP<<" DISCONNECT message: <"<<messageReceived<<"> received from " << cIP<< " ....." <<endl;
                Optimizer::Inst()->removeChild(cIP);

                {
                    string debugMsg="removeEdge 0-0 "+receiverIP+" "+cIP+"";
                    this->sendDebugMessage(debugMsg);
                }

                MediaForwarder::Inst()->removeDestination(cIP);

                for (list<TimeOut>::iterator it = keepAliveList.begin(); it != keepAliveList.end(); it++)
                {


                    if ((*it).destAddr==cIP)
                    {
                       cout<<"@"<<receiverIP<<" erase " << cIP<< " in keep alive list....." <<endl;
                       it= keepAliveList.erase(it);

                    }
                }
                if (Optimizer::Inst()->treeBalancing()!="1")
                {
                    cout<<"@"<<receiverIP<<": Tree banlancing result is:"<<Optimizer::Inst()->treeBalancing()<<"..."<<endl;
                    this->sendMessage(Optimizer::Inst()->treeBalancing(),0,TREEBALANCE,receiverIP);
                }


                if(!ifSource)
                {
                    this->sendMessage(Optimizer::Inst()->getParent(),0,TOPOUPDATE,messageReceived);
                }
                Optimizer::Inst()->printTopoInfo();
            }
                break;
            case TOPOUPDATE://TOPOUPDATE message received from a child
            {
                cout<<"@"<<receiverIP<<" TOPOUPDATE message: <"<<messageReceived<<"> received from " << cIP<< " ....." <<endl;
                if(!ifSource)
                {
                    this->sendMessage(Optimizer::Inst()->getParent(),0,TOPOUPDATE,messageReceived);
                }
                if (messageReceived!="1")
                    {
                       int noc=atoi(messageReceived.c_str());
                       for (int i =0; i<noc-1;i++)
                          Optimizer::Inst()->childRemovedAt(cIP);

                    }
                    if (Optimizer::Inst()->childRemovedAt(cIP))
                    {
                        if (Optimizer::Inst()->treeBalancing()!="1")
                        {
                            cout<<"@"<<receiverIP<<": Tree banlancing result is:"<<Optimizer::Inst()->treeBalancing()<<"..."<<endl;
                            this->sendMessage(Optimizer::Inst()->treeBalancing(),0,TREEBALANCE,receiverIP);
                        }
                        else
                        {
                            cout<<"Tree is balanced already, no need to repair.."<<endl;
                        }
                    }
                Optimizer::Inst()->printTopoInfo();

            }
                break;
            case TREEBALANCE://TREEBANLANCE message received
            {
                cout<<"@"<<receiverIP<<" TREEBANLANCE message: <"<<messageReceived<<"> received from " << cIP<< " ....." <<endl;
                Optimizer::Inst()->setParent(cIP);
                if (Optimizer::Inst()->findLeaf()=="1")
                {
                    cout<<"This is a leaf child..."<<endl;

                    this->sendMessage(Optimizer::Inst()->getParent(),0,DISCONNECT,"1");
                    for (list<TimeOut>::iterator it = keepAliveList.begin(); it != keepAliveList.end(); it++)
                    {


                        if ((*it).destAddr==Optimizer::Inst()->getParent())
                        {
                           cout<<"@"<<receiverIP<<" erase " << Optimizer::Inst()->getParent()<< " in keep alive list....." <<endl;
                           it= keepAliveList.erase(it);

                        }
                    }
                    Optimizer::Inst()->setParent("");
                   // sleep(1);

                    this->sendMessage(srcAddr,mediaPort,DROP,receiverIP);

                }
                else
                {
                    cout<<"Finding the leaf child.."<<Optimizer::Inst()->findLeaf()<<endl;
                    this->sendMessage(Optimizer::Inst()->findLeaf(),0,TREEBALANCE,messageReceived);
                }
                Optimizer::Inst()->printTopoInfo();

            }
                break;
            case KEEPALIVE://KEEPALIVE message  received
            {
               // cout<<"@"<<receiverIP<<" KEEPALIVE message received from " << cIP<< " ....." <<endl;
                if (!ifCheckAliveEstablished)
                {
                    pthread_t checkAlive;
                    int rc2 = pthread_create(&checkAlive, NULL, checkalive,(void*)NULL);
                    if (rc2){
                    printf("ERROR; return code from pthread_create() is %d\n", rc2);
                    exit(-1);
                    }
                    if (!ifKeepAliveEstablished)
                    {
                        pthread_t keepAlive;
                        rc2 = pthread_create(&keepAlive, NULL, keepalive,(void*)NULL);
                        if (rc2){
                        printf("ERROR; return code from pthreadcIP_create() is %d\n", rc2);
                        exit(-1);
                        }
                        ifKeepAliveEstablished=true;
                    }
                    ifCheckAliveEstablished=true;
                }

                if (keepAliveList.size()>0)
                {
                    for (list<TimeOut>::iterator it = keepAliveList.begin(); it != keepAliveList.end(); it++)
                    {

                        //cout<<"Erase KeepAliveList Item: "<<(*it).destAddr<<endl;
                        if ((*it).destAddr==cIP)
                        {
                           it= keepAliveList.erase(it);
                        }
                    }
                }
                time_t now;
                TimeOut item;
                time (&now);
                item.destAddr=cIP;
                item.lastUpdate=now;
                keepAliveList.push_back(item);
               /* for (list<TimeOut>::iterator it = keepAliveList.begin(); it != keepAliveList.end(); it++)
                {
                    cout<<"Keepalive destination: "<<(*it).destAddr<<" lastUpdate "<< (*it).lastUpdate<<endl;
                }*/
            }
                break;
            default :// unrecognized message received
                break;
        }


    }
    close(sockfd);
}

string ControlMessenger::getSelfIP()
{
    return selfIP;
}
void ControlMessenger::sendMessage(string receiverIP,int prt, int tp,string messageToSend)
{


    // cout<<"Accessing sendMessage of singleton instance of ControlMessenger...."<<endl;

    Message *msg = new Message(receiverIP,prt,tp);


    int sock;
    struct sockaddr_in server_addr;
    struct hostent *host;
    string send_data;
   // string str_sdp;
    //define the IP address of the source

    host= (struct hostent *) gethostbyname(receiverIP.c_str());
    if (host==NULL){
        //assert(host);
        cout<<"Cannot send message "<<prt<<" "<<tp<<" "<<messageToSend<<" "<<"to "<<receiverIP<<endl;
    }
    else{
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
        perror("socket");
        cout<<"Not able to send "<<send_data<<" at sendMessage"<<endl;
        exit(1);
        }

        server_addr.sin_family = AF_INET;

        //define the control messaging port of the server
        server_addr.sin_port = htons(EnvironmentalSettings::Inst()->SIGNALING_PORT);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);

        bzero(&(server_addr.sin_zero),8);
        send_data=msg->toString()+" "+messageToSend;

        const char *p = send_data.c_str();
        if (tp!=KEEPALIVE){
            cout<<"<<<<<<<<<<<<<The message **"<<p<<"** is sent>>>>>>>>>>>>>"<<endl;
        }
        sendto(sock, p, strlen(p), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

        msg= NULL;
        delete msg;
        close(sock);
    }

}

void ControlMessenger::setFilePath(string fp)
{
    filePath=fp;
}
string ControlMessenger::getFilePath()
{
    return filePath;
}



void *media_Forwarder(void* args)
{
    MediaForwarder *m = (MediaForwarder *)args;
    assert(m);
    cout<<"The thread media forwarder is established at source side with file:"<<m->getFilePath()<<"..."<<endl;
    m->startStreaming(m->getFilePath());
    return NULL;
}



void *send_sdp(void *args)
{
    args=NULL;
    int sock1,sock2;
    socklen_t clength;
    sock1 =  socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv,cli;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(EnvironmentalSettings::Inst()->SDPPORT);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock1,(struct sockaddr *)&serv, sizeof(serv));
    listen(sock1,5);
    clength = sizeof(cli);
    char buf[1024];
    sock2 = accept(sock1,(struct sockaddr *)&cli,&clength);
    printf("\n Client Connected\n");
    FILE* fp = fopen("session.sdp","r");
    printf("I am here outside while loop \n");
     while(!feof(fp)){
            //bzero(buf,sizeof(buf));
            fread(buf,sizeof(char),50,fp);
            write(sock2,buf,50);
    }
    write(sock2,"quit1234",50);
    fclose(fp);
    close(sock1);
    close(sock2);

    return NULL;

}
void receive_sdp(string ip_add)
{
    int sock1;
    sock1 =  socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv;
    serv.sin_port = htons(EnvironmentalSettings::Inst()->SDPPORT);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(ip_add.c_str());
    printf("client connecting\n");
    connect(sock1, (struct sockaddr *)&serv,sizeof(serv));
    char buf[1024];
    if( remove( "session.sdp" ) == -1 )
            perror( "Error deleting session.sdp" );
      else
            puts( "File deleted" );
    FILE* fp = fopen("session.sdp","w");
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
    close(sock1);
}

void *play(void *args)
{
    args=NULL;
    if (EnvironmentalSettings::Inst()->LABMODE)
    {
        /*string input;
        cout<<"Enter q to quit: "<<endl;
        cin>>input;

        if (input=="q"){return NULL;}*/
    }
    else
    {
        string cmdMP4="MP4Client C session.sdp";
        system(cmdMP4.c_str());
    }
    return NULL;
}

void *goodbye(void *args)
{
    pthread_t playThread=*((pthread_t*) args);
    void* endSign;
    pthread_join(playThread,&endSign);
    cout<<"The goodbye thread is created............"<<endl;

    cout<<"The parent Address is "<<Optimizer::Inst()->getParent()<<"............"<<endl;
    stringstream stream;
    int noc=atoi(Optimizer::Inst()->getNumberofChild().c_str());
    string str_noc;
    stream<< noc+1;
    stream>> str_noc;
    ControlMessenger::Inst()->sendMessage(Optimizer::Inst()->getParent(),0,DISCONNECT,str_noc);
    list<PeerinfoItem> children=Optimizer::Inst()->quitStreaming();
    for (list<PeerinfoItem>::iterator it = children.begin(); it != children.end(); it++)
    {
        cout<<"The direct children are:"<<(*it).childIP<<":"<<(*it).childPort<<endl;
        ControlMessenger::Inst()->sendMessage(ControlMessenger::Inst()->getSrcAddr(),(*it).childPort,DROP,(*it).childIP);
    }
    list<TimeOut> keepAliveList= ControlMessenger::Inst()->getKeepAliveList();
    for (list<TimeOut>::iterator it = keepAliveList.begin(); it != keepAliveList.end(); it++)
    {


        if ((*it).destAddr==Optimizer::Inst()->getParent())
        {
           it= keepAliveList.erase(it);
        }
    }
    ControlMessenger::Inst()->setKeepAliveList(keepAliveList);
    return NULL;
}
void *keepalive(void *args)
{
    args=NULL;
    cout<<"The keepalive thread is created............"<<endl;
    while (true)
    {
        srand(time(NULL));
        float sleepTime=EnvironmentalSettings::Inst()->KEEPALIVE_INTERVAL+(rand()%10)/100;
        sleep(sleepTime);
        if (!ControlMessenger::Inst()->getIfSrc())
        {
            if (Optimizer::Inst()->getParent()!="")
            {
                ControlMessenger::Inst()->sendMessage(Optimizer::Inst()->getParent(),0,KEEPALIVE,"KEEPALIVE");
            }
        }
        list<PeerinfoItem> children=Optimizer::Inst()->getDirectChildren();
        if (children.size()>0){
            for (list<PeerinfoItem>::iterator it = children.begin(); it != children.end(); it++)
            {
                //cout<<"The direct children are:"<<(*it).childIP<<":"<<(*it).childPort<<endl;
                ControlMessenger::Inst()->sendMessage((*it).childIP,0,KEEPALIVE,"KEEPALIVE");
            }
        }
    }

    return NULL;
}
void *checkalive(void *args)
{
    args=NULL;
    cout<<"The checkalive thread is created............"<<endl;

    while (true)
    {

        list<TimeOut> keepAliveList= ControlMessenger::Inst()->getKeepAliveList();
        if (keepAliveList.size()>0)
        {
            list<TimeOut>::iterator it = keepAliveList.begin();
            TimeOut earlistKeepalive= *it;
           // cout<<"The oldest keep alive peer is:"<<earlistKeepalive.destAddr<<endl;
            time_t now;
            time (&now);
            if (difftime(now,earlistKeepalive.lastUpdate)>EnvironmentalSettings::Inst()->TIMEOUT_INTERVAL)
            {
                cout<<"Connection lost to peer: "<<earlistKeepalive.destAddr<<endl;
                it= keepAliveList.erase(it);
                ControlMessenger::Inst()->setKeepAliveList(keepAliveList);
                string receiverIP=ControlMessenger::Inst()->getSelfIP();

                if (earlistKeepalive.destAddr==Optimizer::Inst()->getParent())
                {
                    {
                        string debugMsg="removeEdge 0-0 "+earlistKeepalive.destAddr+" "+receiverIP+"";
                        ControlMessenger::Inst()->sendDebugMessage(debugMsg);
                    }

                    string data=receiverIP+"_"+Optimizer::Inst()->getNumberofChild()+"";
                    sleep(2*EnvironmentalSettings::Inst()->TIMEOUT_INTERVAL);
                    ControlMessenger::Inst()->sendMessage(ControlMessenger::Inst()->getSrcAddr(),MediaForwarder::Inst()->getMediaPort(),DROP,data);

                }
                else
                {
                    string str_noc;
                    stringstream stream;
                    stream<< Optimizer::Inst()->getNocOfChild(earlistKeepalive.destAddr)+1;
                    stream>> str_noc;
                    {
                        string debugMsg="removeEdge 0-0 "+receiverIP+" "+earlistKeepalive.destAddr+"";
                        ControlMessenger::Inst()->sendDebugMessage(debugMsg);
                    }
                    Optimizer::Inst()->removeChild(earlistKeepalive.destAddr);
                    //ControlMessenger::Inst()->sendMessage(receiverIP,MediaForwarder::Inst()->getMediaPort(),DROP,earlistKeepalive.destAddr);

                    MediaForwarder::Inst()->removeDestination(earlistKeepalive.destAddr);
                    if(!ControlMessenger::Inst()->getIfSrc())
                    {

                        ControlMessenger::Inst()->sendMessage(Optimizer::Inst()->getParent(),0,TOPOUPDATE,str_noc);
                        //sleep(1);
                    }
                    //sleep(EnvironmentalSettings::Inst()->TIMEOUT_INTERVAL);
                    if (Optimizer::Inst()->treeBalancing()!="1")
                    {
                        string treeBalance=Optimizer::Inst()->treeBalancing();
                        cout<<"@"<<receiverIP<<": Tree banlancing result is:"<<treeBalance<<"..."<<endl;

                        ControlMessenger::Inst()->sendMessage(treeBalance,0,TREEBALANCE,receiverIP);
                    }
                }


            }
        }
        sleep(EnvironmentalSettings::Inst()->TIMEOUT_INTERVAL);
    }

    return NULL;
}
list<TimeOut> ControlMessenger::getKeepAliveList()
{
    return keepAliveList;
}
void ControlMessenger::setKeepAliveList(list<TimeOut> kpList)
{
    keepAliveList=kpList;
}
void ControlMessenger::sendDebugMessage(string send_data)
{
    // cout<<"Accessing sendMessage of singleton instance of ControlMessenger...."<<endl;
    int sock;
    struct sockaddr_in server_addr;
    string dest=EnvironmentalSettings::Inst()->DEBUGSERVER;
    /*struct hostent *host;
    //define the IP address of the source
    host= (struct hostent *) gethostbyname();
    assert(host);*/
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        cout<<"Not able to send debug Message: "<<send_data<<endl;
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    //define the control messaging port of the server
    server_addr.sin_port = htons(EnvironmentalSettings::Inst()->DEBUGPORT);
    server_addr.sin_addr.s_addr= inet_addr(dest.c_str());
   // server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero),8);
    const char *p = send_data.c_str();
    cout<<"<<<<<<<<<<<<<The message **"<<p<<"** is sent to "<<EnvironmentalSettings::Inst()->DEBUGSERVER<<">>>>>>>>>>>>>"<<endl;
    sendto(sock, p, strlen(p), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    close(sock);
}
