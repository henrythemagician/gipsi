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
    \file EnvironmentalSettings.h

    \ brief describes the singleton class EnvironmentalSettings, which is
    responsible for providing global access of environmental settings, and
    include most used standard libs
*/

#ifndef ENVIRONMENTSETTINGS_H
#define ENVIRONMENTSETTINGS_H
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
#include <list>
#include <map>
#include <cassert>
#include <fstream>
#include <stddef.h>
#include <assert.h>
#include "vector"
using namespace std;

#define CONNECT 1
#define DROP 2
//#define ACCEPT 3
#define DISCONNECT 3
#define TOPOUPDATE 4
#define TREEBALANCE 5
#define KEEPALIVE 6
#define MEDIAMTU 1462

class EnvironmentalSettings {
    public:
        int SDPPORT;
        int SIGNALING_PORT;
        int UPLIMIT;
        int DEBUGPORT;
        float KEEPALIVE_INTERVAL;
        float TIMEOUT_INTERVAL;
        bool LABMODE;
        string DEBUGSERVER;
        static EnvironmentalSettings* Inst();
        ~EnvironmentalSettings();
    protected:
         EnvironmentalSettings();
    private:
        static EnvironmentalSettings* pEnvSetting;
 };



#endif // ENVIRONMENTSETTINGS_H

