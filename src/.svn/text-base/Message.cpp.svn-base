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


#include "Message.h"


Message::Message(string ip,int prt, int tp)
{
    dstIP=ip;
    port=prt;
    msgType=tp;
}

 string Message::toString()
 {
     string msg,str_msgType,str_port;


     //conversion from int to string
     stringstream stream1,stream2;
     stream1<< msgType;
     stream1>> str_msgType;
     stream2<< port;
     stream2>> str_port;

     msg = dstIP+" "+str_port+" "+str_msgType;

     return msg;

 }
