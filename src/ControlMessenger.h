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
    \file ControlMessenger.h

    \ brief describes the singleton class ControlMessenger, which is
    responsible for receiving and sending control messengers for gipsi
*/

#ifndef CONTROLMESSENGER_H
#define CONTROLMESSENGER_H

#include "EnvironmentSettings.h"


#include <MediaForwarder.h>

using namespace std;



struct TimeOut
{
    string destAddr;
    time_t lastUpdate;
};

class ControlMessenger
{
public:


    static ControlMessenger* Inst();

    /* initialization sendMessage
    * This function is responsible for sending the messages on given IP and port
    * @param string int int string
    * @return void
    */
    void sendMessage(string ip,int prt, int tp,string messageToSend);

    /* initialization Initialize
    * @param int
    * @return void
    */
    void Initialize(int prt);

    /* initialization setPort
    * @param int
    * @return void
    */
    void setPort(int prt);

    /* initialization getPort
    * @param int
    * @return int
    */
    int getPort();

    /* initialization setSource
    * This function is responsible for setting the source
    * @param bool
    * @return void
    */
    void setSource(bool ifsrc);

    /* initialization getIfSrc
    * This function return the source
    * @param
    * @return void
    */
    bool getIfSrc();

    /* initialization setFilePath
    * This function set the path for media file.
    * @param string
    * @return void
    */
    void setFilePath(string fp);

    /* initialization getFilePath
    * This function return the path for media file.
    * @param
    * @return string
    */
     string getFilePath();

     /* initialization ~ControlMessenger
     * This is a destructor for the object of class ControlMessenger
     * @param
     * @return
     */
     ~ControlMessenger();

     /* initialization setIfSrc
     * This function set the source
     * @param bool
     * @return void
     */
    void setIfSrc(bool IF);

    /* initialization setSrcAddr
    * This function set the address of the media source
    * @param string
    * @return void
    */
    void setSrcAddr(string addr);

    /* initialization getSrcAddr
    * This function return the address of the source
    * @param
    * @return string
    */
    string getSrcAddr();

    /* initialization setMediaPort
    * This function set the port for media transferring.
    * @param int
    * @return void
    */
    void setMediaPort(int prt);

    /* initialization sendDebugMessage
    * @param string
    * @return void
    */
    void sendDebugMessage(string debugMsg);

    /* initialization MessageListener
    * This function handles the incoming messages
    * @param
    * @return void
    */
    void messageListener();

    /* initialization getKeepAliveList...............
    * @param
    * @return void
    */
    list<TimeOut> getKeepAliveList();

    /* initialization setKeepAliveList...............
    * @param list<TimeOut>
    * @return void
    */
    void setKeepAliveList(list<TimeOut> kpList);

    /* initialization getSelfIP................
    * @param
    * @return void
    */
    string getSelfIP();

    /* initialization getMediaPort...........
    * @param
    * @return void
    */
    int getMediaPort();
    bool ifParentChange;
protected:

    /* initialization ControlMessenger
    * @param int
    * @return
    */
    ControlMessenger(int prt);
private:
    static ControlMessenger* pCtrlMsger;
    int signalingPort;
    bool ifSource;
    string filePath;
    bool ifFirstPeer;
    string srcAddr;
    int mediaPort;
    list<TimeOut> keepAliveList;
    bool ifCheckAliveEstablished;
    string selfIP;
    bool ifKeepAliveEstablished;


};

#endif // CONTROLMESSENGER_H
