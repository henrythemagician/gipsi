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
    \file Message.h

    \ provides a message type for sending control message
*/


#ifndef MESSAGE_H
#define MESSAGE_H
using namespace std;

#include "EnvironmentSettings.h"

class Message
{
public:

    /* initialization Message
    * @param string int int
    * @return void
    */
    Message(string dstIP,int prt, int tp);

    /* initialization toString
    * This function is used to conver the message to string.
    * @param int string
    * @return string
    */
    string toString();

private:
    int port;
    string dstIP;
    int msgType;

};

#endif // MESSAGE_H
