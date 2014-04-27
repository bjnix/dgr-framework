DGR Framework (DISTRIBUTED GRAPHICS RENDERER) (pronounced "Dogger")
---------------------------------------------------------
Written by Brent Nix (bjnix at mtu dot edu) and James Walker (jwwalker at mtu dot edu),
with some modifications by Scott A. Kuhl (kuhl at mtu dot edu)

OVERVIEW
--------
Originaly meant for distributed graphics rendering, DGR is a small lightweight general use framework that allows for syncing data between servers with minimal code intrusion. It takes primitive data types by default, but can be configured to accept any data type, structure, or object.


HOW IT WORKS
------------------
Here is an overview of how DGR works:

The MASTER program forwards state data via UDP packets to the relay
program running on the head node of the cluster.

The RELAY, running on the head node of the cluster, listens for
packets from the master program and automatically forwards them to an
IP address (which could be a broadcast address, and is by default).

The SLAVES listen for packets on the appropriate port. Upon receipt, they
immediately decode the packets and update their state with the data.

The basic premise here is a master - slave relationship. The specifics of how this relationship is implemented is completely up to you, but examples are provided for some more common constructs. Here are the examples that we have implemented:

1) Having a master node that talks to a relay on a cluster head

                                                               {slave}
                                                               {slave}
                                          {Master}--->{relay}--{slave}
                                                               {slave}
                                                               {slave}

2) Running directly off of the cluster head (the relay functionality is moved into the master code)

                                                               {slave}
                                                               {slave}
                                             {master/relay}----{slave}
                                                               {slave}
                                                               {slave}
        
and finally, a purely local environment allows us to recreate the above situations for local testing purposes.

                                                    master
                                          {relay,slave,slave,slave,...}

                                                      or

                                                 master/relay
                                          {slave,slave,slave,slave,...}




 Some sample "run-scripts" have been included to help you get started:

Scripts specifically for Michigan Tech's IVS Display Wall:

-  DGRStartIVS.sh 
-  DGRStartIVS-startslaves.sh
   note: DGRStartIVS.sh calls DGRStartIVS-startslaves.sh

For testing on one machine:
  
- DGRStartLocal.sh
  note: this may need some serious editing for your purposes
 
TO COMPILE THE PACKAGES
-----------------------
- Compile the package by typing "make".
- testing_around.cpp has been provided as an example
- This code should work with linux and OSX.
- it has not been tested on windows and will likely not work without modification

DEVELOPING YOUR OWN DISTRIBUTED RENDERING PROGRAMS
--------------------------------------------------
- The testing_around.cpp demo illustrates how to accomplish distributed rendering on a graphics
cluster using only UDP packets, without relying on libraries such as
Chromium. 
- DGR is independent of the windows composter or graphics libraries so it allows you to sync renderings between different windows managers and operating systems, having separate slave implementations for each.
- Because DGR is a memory synchronizer, you can extend this concept for any distributed computation purposes, not just graphics. 
- You must include "DGR_framework" in the files that you wish to use DGR in as well as compile your code with "DGR_framework.cpp"  
 
- Because the source code for the master and slaves may be almost identical, you can have both binaries compiled out of the same source file. The code below demonstrates how to use preprocessor directives to separate out parts that are different between the master and the slave. I don't think that I have to mention that some care should be taken when choosing how to cut the code up.
__note:__ _use -DDGR_MASTER=1 with gcc for the MASTER and omit it for the SLAVE._
````cpp
      #ifdef DGR_MASTER
        //C++ code for the master program
      #else
        //C++ code for the slave program
      #endif
      
      #ifdef DGR_MASTER
         //C++ code for the master program
      #endif
      
      #ifndef DGR_MASTER
        //C++ code for the SLAVE program. #if*n*def means "if not defined"
      #endif
````


- The way that DGR must be instantiated is different for the MASTER and SLAVE nodes
(I.E. inside the preprocessor `#ifdef` statements:
__For the MASTER:__ 
````cpp
DGR_framework myDGR = new DGR_framework(char * relay_IP);
````

 where `relay_IP` is the relay address in a __character array__

 __For the SLAVE:__
````cpp
DGR_framework myDGR = new DGR_framework();
````

 which will set the slave listening port to RELAY_LISTEN_PORT defined in DGR_framework.h

 __or__

 If you would like to specify the listening port, do it like this:
````cpp
DGR_framework myDGR = new DGR_framework(int slave_listen_port);
````

- Variable registration __must __ be in both MASTER and SLAVE code. 
Future versions will have more robust error checking, but failing to have variables added on both MASTER and SLAVE 
nodes may cause errors.
(I.E. not inside of any of the preprocessor "ifdef" statements):
 
 ````cpp
myDGR->addNode<T>(std::string name, *T data)
````
__note:__ _the above method will only work with POD (Plain Old Data) types _

 In order to sync other types, you have to specialize templates. The following code is an example from fitashape,
a game made to be run on the Michigan Tech IVS display wall.

 First, _serialize_ the data, copying each into the necessary array:
 ````cpp
template<>
char * MapNode<Player>::getDataString(char* data_array){
//pulling the data out of the object
  std::vector<vector3df> dataPos = data->getPositions();
//putting that data into an array
  float float_array[12];
  dataPos[0].getAs3Values( &( float_array[0] ) );
  dataPos[1].getAs3Values( &( float_array[3] ) );
  dataPos[2].getAs3Values( &( float_array[6] ) );
  dataPos[3].getAs3Values( &( float_array[9] ) );
//copying that data into the given buffer    
    memcpy(data_array, float_array, dataLength);
}
````
 Next, reverse the process and _parse_ the data back into the object from the array that you just made:
 ````cpp
template<>
void MapNode<Player>::setData(char * data_array){
  float float_array[24];
//copy raw data into "datatyped" array
    memcpy(float_array, data_array, dataLength);
    std::vector<vector3df> dataPos;
//object specific parsing
    dataPos.push_back(vector3df(float_array[0],float_array[1],float_array[2]));
    dataPos.push_back(vector3df(float_array[3],float_array[4],float_array[5]));
    dataPos.push_back(vector3df(float_array[6],float_array[7],float_array[8]));
    dataPos.push_back(vector3df(float_array[9],float_array[10],float_array[11]));
    data->setNodePositions(dataPos);
}
````

- To guarantee good performance, YOU MAY WANT TO CAP THE
RATE AT WHICH THE MASTER SENDS UDP PACKETS TO ABOUT 60 PER SECOND, but
feel free to experiment. 

- Please note that for more complex programs, master-defined state machines are necessary. 
Without them you will veritably run into race conditions. 
__Software using DGR must be made in a concurrent programming paradigm.__

- You will need a simple relay program. DGR_relay.cpp is provided as an example. The relay's ONLY responsibility is to
RECEIVE packets from the MASTER and immediately SEND those packets to the
SLAVES (for greatest efficiency, just broadcast them). DGR_relay accepts the sendto IP address and can also accept
SLAVE listening ports

- The SLAVES receive state data generated by the MASTER and use that to
update their displays. When initialized, the slave must be given VIEWPORT
INFORMATION telling it WHICH PART of the display it is responsible for (e.g.,
the upper-left, the middle-right, etc.). This is what enables the slaves to
correctly divide their views using the glFrustum function (the D3D equivalent is D3DXMatrixPerspectiveOffCenterLH). 
See the demo for an example.

- The relay and slaves are set up to automatically terminate themselves if
they haven't received any packets in a while, so you should only need to
worry about closing the master program. However, killSlaves.sh has been provided as a catchall 
Michigan Tech's IVS wall if anything should go wrong.


SUPPORT
-------
If you have questions, comments, or need assistance with this demo or
using the demo to distribute rendering in your own programs, please
contact

- bjnix at mtu dot edu << currently maintains this repo (_direct questions here first_)
_and/or_
- jwwalker at mtu dot edu
 _and/or_
- kuhl at mtu dot edu
