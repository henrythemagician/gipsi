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

#include "EnvironmentSettings.h"



EnvironmentalSettings* EnvironmentalSettings::pEnvSetting = NULL;

EnvironmentalSettings::EnvironmentalSettings()
{
    SDPPORT = 3000;
    SIGNALING_PORT = 40000;

    UPLIMIT = 2;
    LABMODE = true;
    DEBUGPORT = 20000;
    KEEPALIVE_INTERVAL = 1.0;
    TIMEOUT_INTERVAL = 2.0;
    DEBUGSERVER = "141.24.207.42";

}

EnvironmentalSettings* EnvironmentalSettings::Inst()
{
   if(pEnvSetting == NULL)
    {
      pEnvSetting = new EnvironmentalSettings();
    }
   //assert(pEnvSetting);
   return pEnvSetting;
}

EnvironmentalSettings::~EnvironmentalSettings()
{
    pEnvSetting = NULL;
    delete pEnvSetting;
}
