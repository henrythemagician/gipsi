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

#include "GipsiController.h"



void *call_Listener(void *args);


GipsiController::GipsiController()
{
}

void GipsiController::connect(string ip, int prt)
{

    ControlMessenger::Inst()->setIfSrc(false);
    ControlMessenger::Inst()->setSrcAddr(ip);
    ControlMessenger::Inst()->setMediaPort(prt);
     MediaForwarder::Inst()->setMediaPort(prt);
    pthread_t server_thread;
    int rc;
    rc = pthread_create(&server_thread, NULL, call_Listener, (void *)ControlMessenger::Inst());
    if (rc)
    {
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
    }
    //sleep(2);
    ControlMessenger::Inst()->sendMessage(ip,prt,CONNECT,"request");
    MediaForwarder::Inst()->signalingFrom(ip,prt);
    MediaForwarder::Inst()->receiveFrom(prt);

}

string GipsiController::openAsSrc(string filePath)
{


    string output;

    pthread_t threadCM;

    int rc;
    ControlMessenger::Inst()->setIfSrc(true);
    ControlMessenger::Inst()->setFilePath(filePath);
    rc = pthread_create(&threadCM, NULL, call_Listener, (void *)ControlMessenger::Inst());
    if (rc)
    {
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
    }

    output = "Starting streaming "+ filePath+" .....";
    return output;
}

void *call_Listener(void *args)
{
    args=NULL;
    /*if (!ControlMessenger::Inst()->getIfSrc())
    {
        string ip=ControlMessenger::Inst()->getSrcAddr();
        int prt=ControlMessenger::Inst()->getMediaPort();
        int tp=1;
        ControlMessenger::Inst()->sendMessage(ip,prt,tp,"request");
    }*/
    cout<<"The thread call listener at port "<<EnvironmentalSettings::Inst()->SIGNALING_PORT<<" is established at server side...."<<endl;
    ControlMessenger::Inst()->messageListener();
    return NULL;
}

