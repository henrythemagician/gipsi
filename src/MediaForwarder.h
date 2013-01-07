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

/*
    \file MediaForwarder.h

    \ This header file describes the class MediaForwader, which is
    responsible for sending and receiving the media stream between peers
*/


#ifndef MEDIAFORWARDER_H
#define MEDIAFORWARDER_H

using namespace std;
#include "EnvironmentSettings.h"



struct Sock
{
    string ip;
    int port;
    int socket;
};

struct ForwardSock
{
    int srcPort;
    list <struct Sock> dests;

};


class MediaForwarder
{
public:

    static MediaForwarder* Inst();

    /* initialization startStreaming
    * This function is used to start the stream from the given file path
    * @param string
    * @return void
    */
    void startStreaming(string fp);

    /* initialization addAudience
    * This function is used to add the clients to the parent.
    * @param string int
    * @return void
    */
    void addAudience(string ip,int port);

    /* initialization setMediaPort
    * This function is used to set the port for media transferring
    * @param int
    * @return void
    */
    void setMediaPort(int mPrt);

    /* initialization setFilePath
    * This function is used to setup a path to media file
    * @param string
    * @return void
    */
    void setFilePath(string fp);

    /* initialization getMediaPort
    * This function returns the media port
    * @param
    * @return int
    */
    int getMediaPort();

    /* initialization getFilePath
    * This function return the path to the media file.
    * @param
    * @return string
    */
    string getFilePath();

    /* initialization receiveFrom
    * This Function is used to receive media from the given IP and port
    * @param string int
    * @return void
    */
    void receiveFrom(int prt);

    /* initialization singnalingFrom
    * This Function is used to recevie the signals from the given IP and port
    * @param string int
    * @return void
    */
    void signalingFrom(string ip,int prt);

    /* initialization ~MediaForwarder
    * @param
    * @return
    */
    ~MediaForwarder();

    /* initialization forwardMedia
    * This Function is used to forward the media to a specific port for the child node
    * @param int
    * @return void
    */
    void forwardMedia(int srcPort);

    /* initialization addChild
    * This Function is used to add the childs for sigaling
    * @param string int
    * @return void
    */
    void addChild(string ip,int port);

    /* initialization checkSrcPort
    * This Function is used to check if the port exist for the use
    * @param int
    * @return bool
    */
    bool checkSrcPort(int port);

    /* initialization addChildAsForwarder
    * This Function is used to add the child as intermediate node
    * @param string int
    * @return void
    */
    void addChildAsForwarder(string ip,int prt);

    /* initialization removeDestination
    * This Function is used by the parent to remove the destination child
    * @param string
    * @return void
    */
    void removeDestination(string ip);


    /* initialization printFwdSocks
    * @param string
    * @return void
    */
    void printFwdSocks();


protected:
     MediaForwarder();
private:
    static MediaForwarder* pMdFrwder;
    int mediaPort;
    string filePath;
   list<struct ForwardSock> fwdSocks;
  //   vector<struct ForwardSock> fwdSocks;
};

#endif // MEDIAFORWARDER_H
