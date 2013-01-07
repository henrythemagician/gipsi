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

#include "Optimizer.h"


Optimizer* Optimizer::pOptimizer = NULL;

Optimizer* Optimizer::Inst()
{
    if(pOptimizer == NULL)
    {
        pOptimizer = new Optimizer();
    }
    return pOptimizer;
}

Optimizer::Optimizer()
{
    //cout<<"Access constructor of Optimizer  ....."<<endl;
    ifStarted=false;
    localUploadCapacity=EnvironmentalSettings::Inst()->UPLIMIT;
}

void Optimizer::startStreaming(string parentIP)
{
    ifStarted=true;
    parentAddr= parentIP;
   // cout<<"Access startStreaming of Optimizer  ....."<<endl;
}
 list<PeerinfoItem> Optimizer::quitStreaming()
{
     parentAddr="";
    ifStarted=false;
    return directChildren;
   // cout<<"Access quitStreaming of Optimizer  ....."<<endl;
}

 list<PeerinfoItem> Optimizer::getDirectChildren()
{
    return directChildren;
   // cout<<"Access quitStreaming of Optimizer  ....."<<endl;
}
bool Optimizer::checkLimit()
{
   // cout<<"Access checkLimit of Optimizer  ....."<<endl;
    if (localUploadCapacity<=0) return false;
    else return true;
}
string Optimizer::addChild(string parentIP,string childIP,int cport)
{
    if (!ifStarted) this->startStreaming(parentIP);


    if (checkLimit()){//add child locally
        PeerinfoItem newChild;
        newChild.childIP=childIP;
        //newChild.localUploadCapacity=UPLIMIT;
        newChild.numOfChild=0;
        newChild.childPort=cport;
        directChildren.push_back(newChild);
        localUploadCapacity--;//decrease the uploadcapacity because of adding new child
        return "1";
    }
    else
    {   //check the local peerinfoItem to find a peer to add this child to.
        PeerinfoItem loneliest;
        loneliest.childIP="";
        loneliest.numOfChild=1000;
        for (list<PeerinfoItem>::iterator it = directChildren.begin(); it != directChildren.end(); it++)
       {
            PeerinfoItem item=*it;
            if (item.numOfChild==0)
            {//return the childIP
                return item.childIP;
            }
            //find the loneliest child
            if (item.numOfChild<loneliest.numOfChild)
            {
                loneliest.childIP=item.childIP;
                loneliest.numOfChild=item.numOfChild;
            }

       }
        return loneliest.childIP;
    }

}


void Optimizer::removeChild(string childIP)
{
    for (list<PeerinfoItem>::iterator it = directChildren.begin(); it != directChildren.end(); ++it)
   {
        PeerinfoItem item=*it;

        if (item.childIP==childIP){
            it=directChildren.erase(it);
            cout<<"Removing child:" << childIP<< " at Optimizer  ....."<<endl;
        }
   }
    localUploadCapacity++;
}

void Optimizer::setParent(string newParent)
{
    parentAddr=newParent;
}

string Optimizer::getParent()
{
    return parentAddr;
}
string Optimizer::getNumberofChild(){

    stringstream stream;
    string result;
    int n=(int)directChildren.size();
    for (list<PeerinfoItem>::iterator it = directChildren.begin(); it != directChildren.end(); it++)
   {
        PeerinfoItem item=*it;
        n+=item.numOfChild;
   }
    stream << n;
    stream >> result;
    return result;
}

string Optimizer::treeBalancing()
{
    if (localUploadCapacity>=EnvironmentalSettings::Inst()->UPLIMIT)
    {
        return "1";
    }
    PeerinfoItem minNOC,maxNOC;
    minNOC.childIP="";
    minNOC.numOfChild=1000;
    maxNOC.childIP="";
    maxNOC.numOfChild=0;
    for (list<PeerinfoItem>::iterator it = directChildren.begin(); it != directChildren.end(); it++)
   {
        PeerinfoItem item=*it;

        //find the loneliest child
        if (item.numOfChild<minNOC.numOfChild)
        {
            minNOC.childIP=item.childIP;
            minNOC.numOfChild=item.numOfChild;
        }
        //find the most populated child
        if (item.numOfChild>=maxNOC.numOfChild)
        {
            maxNOC.childIP=item.childIP;
            maxNOC.numOfChild=item.numOfChild;
        }
   }
    if ((this->checkLimit())&&maxNOC.numOfChild>0){//if has upload capacity locally, return most populated child
       return maxNOC.childIP;
    }
    if ((maxNOC.numOfChild-minNOC.numOfChild)>1)
    {
        return maxNOC.childIP;
    }
    else
    {
        return "1";
    }

}

string Optimizer::findLeaf()
{
    if (directChildren.size()==0)
    {
        return "1";
    }
    bool ifChildLeaf;
    PeerinfoItem maxNOC;
    maxNOC.childIP="";
    maxNOC.numOfChild=0;

    for (list<PeerinfoItem>::iterator it = directChildren.begin(); it != directChildren.end(); it++)
   {
        PeerinfoItem item=*it;
        //find the most populated child
        if (item.numOfChild>=maxNOC.numOfChild)
        {
            maxNOC.childIP=item.childIP;
            maxNOC.numOfChild=item.numOfChild;
            ifChildLeaf=false;
        }
   }
    {
        return maxNOC.childIP;
    }
}


int Optimizer::getNocOfChild(string childIP)
{
    for (list<PeerinfoItem>::iterator it = directChildren.begin(); it != directChildren.end(); it++)
   {
        if ((*it).childIP==childIP)
        {//return the childIP
            return (*it).numOfChild;
        }
   }
    return -1;
}
bool Optimizer::childAddedAt(string childIP)
{
    for (list<PeerinfoItem>::iterator it = directChildren.begin(); it != directChildren.end(); it++)
   {
        if ((*it).childIP==childIP)
        {//return the childIP
            (*it).numOfChild++;
            cout<<"The number of children at "<<childIP<<" increased to "<<(*it).numOfChild<<"...."<<endl;
        }
   }
    return true;

}
bool Optimizer::childRemovedAt(string childIP)
{
    for (list<PeerinfoItem>::iterator it = directChildren.begin(); it != directChildren.end(); it++)
   {
        if ((*it).childIP==childIP)
        {//return the childIP

            if ((*it).numOfChild-1<0)
            {
                return false;
            }else
            {
                (*it).numOfChild--;
                cout<<"The number of children at "<<childIP<<" decreased to "<<(*it).numOfChild<<"...."<<endl;
                return true;
            }

        }
   }
    return false;
}
void Optimizer::printTopoInfo()
{
    cout<<"*******************************************************"<<endl;
    cout<<"Parent: "<<parentAddr<<endl;
    cout<<"Available Upload Capacity: "<<localUploadCapacity<<endl;
    cout<<"Number of direct children: "<<directChildren.size()<<endl;
    cout<<"Total number of all successors: "<<this->getNumberofChild()<<endl;
    for (list<PeerinfoItem>::iterator it = directChildren.begin(); it != directChildren.end(); it++)
   {
        PeerinfoItem item=*it;
        cout<<"Child "<<item.childIP<<" has "<<item.numOfChild<<" children...."<<endl;
   }
    cout<<"*******************************************************"<<endl;
}
Optimizer::~Optimizer()
{
    pOptimizer = NULL;
    delete pOptimizer;
}

