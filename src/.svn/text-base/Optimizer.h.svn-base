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
    \file Optimizer.h

    \ This header file describes the class Optimizer, which provides
    key methods for building a balanced tree and maintaining the tree.
*/



#ifndef OPTIMIZER_H
#define OPTIMIZER_H
using namespace std;
#include "EnvironmentSettings.h"


struct PeerinfoItem
{
    string childIP;          //The IP address of this direct child
   // int localUploadCapacity;//The remain upload capacity
    int childPort;
    int numOfChild;         // number of all of the sub-nodes of the current peer
                             //(no matter source or receiver, applied to all the peers.
                             // all sub nodes mean children, grandchildren, grandgrandchildren...and so on)

};





class Optimizer
{
public:
    /* initialization Optimizer.................
    * @param
    * @return
    */
    static Optimizer* Inst();

    /* initialization startStreaming.................
    * @param string
    * @return void
    */
    void startStreaming(string parentIP);//join the streaming, from the parent address

    /* initialization quitStreaming.................
    * @param
    * @return list
    */
     list<PeerinfoItem> quitStreaming(); //quit streaming locally, return the list of direct children

     /* initialization checkLimit.................
     * @param
     * @return bool
     */
     bool checkLimit();      //check if the uploadLimit is reached locally

     /* initialization addChild.................
     * @param string string int
     * @return string
     */
     string addChild(string parentIP,string childIP,int cport);/*A genral way to add a child, will add the child directly
                                                      if there are upload capacity left locally, which will
                                                      return "1".
                                                      If the returned value is not "1",
                                                      the string represents the loneliest child
                                                     */

     /* initialization removeChild.................
     * @param string
     * @return void
     */
     void removeChild(string childIP);     //remove a direct child

     /* initialization ~Optimizer................
     * @param
     * @return
     */
    ~Optimizer();

     /* initialization setParent................
     * @param string
     * @return void
     */
     void setParent(string newParent);

     /* initialization getParent .................
     * @param
     * @return string
     */
    string getParent();

    /* initialization getNumberofChild.................

    * @param

    * @return string

    */
    string getNumberofChild();

    /* initialization childAddedAt.................
    * @param string
    * @return void
    */
    bool childAddedAt(string childIP);

    /* initialization childRemovedAt .................
    * @param string
    * @return void
    */
    bool childRemovedAt(string childIP);

    /* initialization treeBalancing.................
    * @param
    * @return string
    */
    string treeBalancing();

    /* initialization findLeaf.................
    * @param
    * @return string
    */
    string findLeaf();

    /* initialization findLeaf.................
    * @param
    * @return string
    */
    list<PeerinfoItem> getDirectChildren();

    /* initialization findLeaf.................
    * @param
    * @return string
    */
    void printTopoInfo();

    /* initialization findLeaf.................
    * @param
    * @return string
    */
    int getNocOfChild(string childIP);
protected:

    Optimizer();


private:
    static Optimizer* pOptimizer;
    bool ifStarted;                      //A flag indicating if the steaming started or not
    //string sourceAddr;                 // ip address of the source
    string parentAddr;                   // ip address of the parent
    list<PeerinfoItem>  directChildren; // ip addresses of all the direct children
    int localUploadCapacity;            //The remain upload capacity

};

#endif // OPTIMIZER_H
