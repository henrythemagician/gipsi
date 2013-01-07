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

#include <QtGui/QApplication>
#include "MainWindow.h"
#include "ControlMessenger.h"
#include "GipsiController.h"


using namespace std;

void print_usage()
{
    cout<< "\nGIPSI - GPAC Interactive Peer-to-peer Streaming plug-In"<<endl;
    cout<< "\n./gipsi [option] input [option] \n" << endl;
    cout<< " -cmd\t\t Enable command line" << endl;
    cout<< " -gui\t\t Enable Graphical Interface" << endl;
    cout<< " -src\t\t Start Gipsi as a source" << endl;
    cout<< " -peer\t\t Start Gipsi as a peer" << endl;
    cout<< " -file=[name]\t Name of the file when start as source" << endl;
    cout<< " -dst=[ip]\t IP address of source when start as peer" << endl;
    cout<< " -port=[port]\t UDP Port to be used for streaming" << endl;
    cout<< "\n -sdp=[port]\t TCP Port for \"session.sdp\" file transfer:default is 3000" << endl;
    cout<< " -sig=[port]\t UDP Port for Signaling(control messages):default is 40000" << endl;
    cout<< " -upl=[Limit]\t Integer Value for Upload Limit:default is 2" << endl;
    cout<< " -lab=[bool]\t Bool Value for enabling Lab mode:default is true" << endl;
    cout<< " -kpi=[seconds]\t Float value for KeepAlive interval in seconds:default is 1.0"<< endl;
    cout<< " -toi=[seconds]\t Float value for TimeOut interval in seconds:default is 2.0" << endl;
    cout<< " -dbs=[ip]\t IP address of topology debugging server" << endl;
    cout<< " -dbp=[port] \t UDP port of topology debugging server" << endl;
    cout<<" \n Examples:"<<endl;
    cout<<"./gipsi -gui"<<endl;
    cout<<"./gipsi -cmd -src -file=[name]"<<endl;
    cout<<"./gipsi -cmd -peer -dst=[ip] -port=[port]"<<endl;
    cout<<"./gipsi -cmd -src -file=[name] -lam=false"<<endl;
}

/*************************************************************************************/
vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim))
    {
       elems.push_back(item);
    }
    return elems;
}
bool is_valid_ip(string  ipaddr){

    if (ipaddr.length()){
            vector<string> _ip=split(ipaddr,'.');
            if (_ip.size()==4){
                    for (int i=0; i < 4; i++){
                            for (int j=0; j < (int)_ip[i].length(); j++)
                                    if (!isdigit(_ip[i][j])) return false;
                            if ((atoi(_ip[i].c_str()) < 0) || (atoi(_ip[i].c_str()) > 255)) return false;
                    }
            return true;
            }
    }
    return false;
 }
/*********************************************************************************************/

bool is_valid_port(const string &port_num)
{
    const char *tmp = port_num.c_str();
    int tmp1 = atoi(port_num.c_str());
    unsigned int i;

    for(i=0; i < strlen(tmp);i++)
    {
        if(!isdigit(tmp[i]))
            return false;
    }

    if((tmp1 < 1) || (tmp1 > 65535) )
        return false;

    return true;
}

/*********************************************************************************************/

int main(int argc, char *argv[])
{

    int i;


    QApplication a(argc, argv);


    bool commandFound=false;
    MainWindow w;


    if(argc < 2)
       {
        cout<<"use ./gipsi -h for Help"<<endl;
           //getchar();
           exit(0);
       }

    if((strcmp("-h",argv[1]))==0)
       {
           commandFound=true;
           print_usage();
           exit(0);
       }

    if((strcmp("-gui",argv[1]))==0)
       {
           commandFound=true;
           w.show();
       }
    else if ((strcmp("-cmd",argv[1]))==0)
    {
        if(argc < 3)
           {
            cout<<"use ./gipsi -h for Help"<<endl;
               //getchar();
               exit(0);
           }


    if((strcmp("-src",argv[2]))==0)
           {
               GipsiController gipsi;
               string file;
              // cout<<"The file path is "<<argv[3]<<endl;
               if(argc < 4)
                  {
                   cout<<"use ./gipsi -h for Help"<<endl;
                      //getchar();
                      exit(0);
                  }
               if (argc>4)
               {

                   for(i=3 ; i < argc ; i++)
                   {
                       if(strncmp("-file",argv[i],5)==0)
                       {
                          string str(argv[i]);
                          size_t pos;
                          pos = str.find("=");
                          file = str.substr (pos+1);
                          //cout<<file;
                       }
                       else
                       if(strncmp("-sdp",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
 /**************************************************************************************/
                           bool check = is_valid_port(tmp);
                           if(check == false)
                           {
                              cout<<"Wrong Port";
                               exit(0);
                           }
/****************************************************************************************/
                           EnvironmentalSettings::Inst()->SDPPORT = atoi(tmp.c_str());
                           cout <<"The TCP port for SDP file transmission is set to:"<<EnvironmentalSettings::Inst()->SDPPORT <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-sig",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->SIGNALING_PORT = atoi(tmp.c_str());
                           cout <<"The UDP port for control messenging is set to:"<<EnvironmentalSettings::Inst()->SIGNALING_PORT <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-upl",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->UPLIMIT = atoi(tmp.c_str());
                           cout <<"The UPLoad Limit is set to:"<<EnvironmentalSettings::Inst()->UPLIMIT <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-lab",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           if(tmp=="false")
                               EnvironmentalSettings::Inst()->LABMODE = false;
                           cout <<"The Lab mode is set to:"<<EnvironmentalSettings::Inst()->LABMODE <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-dbp",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->DEBUGPORT = atoi(tmp.c_str());
                           cout <<"The UDP port for topology debugging is set to:"<<EnvironmentalSettings::Inst()->DEBUGPORT <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-kpi",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->KEEPALIVE_INTERVAL = atof(tmp.c_str());
                           cout <<"The keepalive interval is set to:"<<EnvironmentalSettings::Inst()->KEEPALIVE_INTERVAL <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-toi",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->TIMEOUT_INTERVAL = atof(tmp.c_str());
                           cout <<"The timeout interval is set to:"<<EnvironmentalSettings::Inst()->TIMEOUT_INTERVAL <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-dbs",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
    /**************************************************************************************/
                           bool check = is_valid_ip(tmp);
                           if(check == false)
                           {
                               cout<<"Wrong IP address";
                               exit(0);
                           }
   /****************************************************************************************/
                           EnvironmentalSettings::Inst()->DEBUGSERVER = tmp;
                           cout <<"The ip address of the topology debugging server is set to:"<<EnvironmentalSettings::Inst()->DEBUGSERVER <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       {
                           cout<<"Unrecognized input"<<endl;
                           cout<<"use ./gipsi -h for Help"<<endl;
                           exit(0);
                       }
                   }
               }
               else if(strncmp("-file",argv[3],5)==0)
               {
                   string str(argv[3]);
                   size_t pos;
                   pos = str.find("=");
                   file = str.substr (pos+1);
                   commandFound=true;
               }

               if (commandFound)
               {
                   string output = gipsi.openAsSrc(file);
                   cout<<output<<endl;
               }
           }

           else if((strcmp("-peer",argv[2]))==0)
           {
               string ip;
               int port;
               if(argc < 5)
               {
                cout<<"use ./gipsi -h for Help";
                   //getchar();
                   exit(0);
               }
               if (argc>5)
               {

                   for(i=4 ; i < argc ; i++)
                   {
                       if(strncmp("-port",argv[i],5)==0)
                       {
                          string str(argv[i]),portstr;
                          size_t pos;
                          pos = str.find("=");
                          portstr =str.substr (pos+1);
                          port=atoi(portstr.c_str());
                          //cout<<file;
                       }
                       else
                       if(strncmp("-dst",argv[i],4)==0)
                       {
                          string str(argv[i]);
                          size_t pos;
                          pos = str.find("=");
                          ip = str.substr (pos+1);
                          //cout<<file;
                       }
                       else
                       if(strncmp("-sdp",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->SDPPORT = atoi(tmp.c_str());
                           cout <<"The TCP port for SDP file transmission is set to:"<<EnvironmentalSettings::Inst()->SDPPORT <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-sig",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->SIGNALING_PORT = atoi(tmp.c_str());
                           cout <<"The UDP port for control messenging is set to:"<<EnvironmentalSettings::Inst()->SIGNALING_PORT <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-upl",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->UPLIMIT = atoi(tmp.c_str());
                           cout <<"The UPLoad Limit is set to:"<<EnvironmentalSettings::Inst()->UPLIMIT <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-lab",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           if(tmp=="false")
                               EnvironmentalSettings::Inst()->LABMODE = false;
                           else if (tmp=="true")
                               EnvironmentalSettings::Inst()->LABMODE = true;
                           else
                           {
                               cout<<"Invalid input"<<endl;
                               cout<<"use ./gipsi -h for Help"<<endl;
                               exit(0);
                           }
                           cout <<"The Lab mode is set to:"<<EnvironmentalSettings::Inst()->LABMODE <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-dbp",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->DEBUGPORT = atoi(tmp.c_str());
                           cout <<"The UDP port for topology debugging is set to:"<<EnvironmentalSettings::Inst()->DEBUGPORT <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-kpi",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->KEEPALIVE_INTERVAL = atof(tmp.c_str());
                           cout <<"The keepalive interval is set to:"<<EnvironmentalSettings::Inst()->KEEPALIVE_INTERVAL <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-toi",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->TIMEOUT_INTERVAL = atof(tmp.c_str());
                           cout <<"The timeout interval is set to:"<<EnvironmentalSettings::Inst()->TIMEOUT_INTERVAL <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       if(strncmp("-dbs",argv[i],4)==0)
                       {
                           string str(argv[i]);
                           size_t pos;
                           pos = str.find("=");
                           string tmp = str.substr (pos+1);
                           EnvironmentalSettings::Inst()->DEBUGSERVER = tmp;
                           cout <<"The ip address of the topology debugging server is set to:"<<EnvironmentalSettings::Inst()->DEBUGSERVER <<"..."<<endl;
                           commandFound=true;
                       }
                       else
                       {
                           cout<<"Unrecognized input"<<endl;
                           cout<<"use ./gipsi -h for Help"<<endl;
                           exit(0);
                       }
                   }
               }
               else if(strncmp("-port",argv[4],5)==0)
               {
                  string str(argv[4]);
                  size_t pos;
                  pos = str.find("=");
                  port = atoi(str.substr (pos+1).c_str());
                  //cout<<file;
               }
               if(strncmp("-dst",argv[3],4)==0)
               {
                  string str(argv[3]);
                  size_t pos;
                  pos = str.find("=");
                  ip = str.substr (pos+1);
                  //cout<<file;
                  commandFound=true;
               }
                if (commandFound)
                {
                    GipsiController gipsi;
                    //string IPAddr=argv[3];
                    //int port=atol(argv[4]);
                    commandFound=true;
                    //cout<<"IPAddr is inputed as "<<IPAddr<<endl;
                    gipsi.connect(ip,port);
                }

           }
    }
    if(!commandFound)
    {
        cout<<"Not sufficient argument provided,try the following command for help"<<endl;
        cout<<"./gipsi -h"<<endl;
        //getchar();
        exit(0);
    }




      return a.exec();
  }
