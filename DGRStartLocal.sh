#!/usr/bin/env bash
#./DGRSlave -3.09 0 0.28 .86 360 225 &
#./DGRRelay 0.0.0.0 &	
#./DGRMaster 0.0.0.0

./DGRSlave -3.09 0 0.28 1.44 360 225 25884&
./DGRSlave -3.09 0 1.44 2.6 360 225 25883&

./DGRSlave 0 3.09 0.28 1.44 360 225 25882&
./DGRSlave 0 3.09 1.44 2.6 360 225 25881&

./DGRRelay 0.0.0.0 25881 25882 25883 25884&	
./DGRMaster 0.0.0.0