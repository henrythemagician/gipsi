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
    \file GipsiController.h

    \ brief  describes the class gipsiController, which is
    responsible for setting up the environment as source or peer for gipsi
*/


#ifndef GIPSICONTROLLER_H
#define GIPSICONTROLLER_H
using namespace std;
#include "EnvironmentSettings.h"

#include "ControlMessenger.h"
#include "MediaForwarder.h"

class GipsiController
{

public:

    /* initialization gipsiController
    * @param
    * @return
    */
    GipsiController();

    /* initialization connect
    * This fucntion is called when the client want to connect to the given IP and port
    * @param string int
    * @return void
    */
    void connect(string ip,int prt);

    /* initialization openAsSrc
    * This function is called when the source want to start MP4Box for streaming the media using GIPSI
    * @param string
    * @return string
    */
    string openAsSrc(string filePath);

    

};



#endif // GIPSICONTROLLER_H
