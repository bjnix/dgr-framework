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




1) From the IVS lab (preferably on CCSR), run the MAIN LAUNCH SCRIPT, DGRStartIVS.sh.

2) The main launch script automatically invokes the MASTER program. Thus, the MASTER
   is running on LOCAL HARDWARE that you have physical access to.

3) The main launch script also automatically invokes the SLAVE LAUNCH script on the
   head node of the IVS cluster.
    2a) The slave launch script automatically launches a copy of the SLAVE program
        on every available graphics node.
    2b) The slave launch script then automatically runs a RELAY which is responsible
        for accepting state data from the MASTER and forwarding it to the SLAVES.


TO COMPILE THE PACKAGES
-----------------------
- Compile the package by typing "make".

- Run STLStartIVS.sh on local hardware (CCSR or a laptop you're running
  in the IVS lab).


DEVELOPING YOUR OWN DISTRIBUTED RENDERING PROGRAMS
--------------------------------------------------
This demo illustrates how to accomplish distributed rendering on a graphics
cluster using only UDP packets, without relying on libraries such as
Chromium. You can use this as an example to add this functionality to your
own programs. Here is a brief explanation of how it works:
-You must modify include DGR___framework" and have "DGR_framework.cpp" which contains the code for both
 the master and the slave computer. Look carefully at the preprocessor
 directives in the code. The can come in different varieties such as

      #ifdef DGR_MASTER
        C++ code for the master program
      #else
        C++ code for the slave program
      #endif
      
      #ifdef DGR_MASTER
         C++ code for the master program
      #endif
      
      #ifndef DGR_MASTER
        C++ code for the SLAVE program. #if*n*def means "if not defined"
      #endif


-The master's responsibility is to forward its current state data via
UDP packets. Which data in your program is necessary for the camera to know
where to display? This is probably position and orientation data. Set up
the master version of your program to send this data via UDP, using this
demo as an example. To guarantee good performance, YOU MAY WANT TO CAP THE
RATE AT WHICH THE MASTER SENDS UDP PACKETS TO ABOUT 60 PER SECOND, but
feel free to experiment.

-You will need a simple relay program. The relay's ONLY responsibility is to
RECEIVE packets from the MASTER and immediately SEND those packets to the
SLAVES (for greatest efficiency, just broadcast them).

-The SLAVES receive state data generated by the MASTER and use that to
update their displays. When initialized, the slave must be given VIEWPORT
INFORMATION telling it WHICH PART of the display it is responsible for (e.g.,
the upper-left, the middle-right, etc.). This is what enables the slaves to
correctly divide their views using the glFrustum function. See the demo for
an example.

-The relay and slaves are set up to automatically terminate themselves if
they haven't received any packets in a while, so you should only need to
worry about closing the master program.



SUPPORT
-------
If you have questions, comments, or need assistance with this demo or
using the demo to distribute rendering in your own programs, please
contact
         bjnix at mtu dot edu

            and/or

         jwwalker at mtu dot edu

            and/or

         kuhl at mtu dot edu
