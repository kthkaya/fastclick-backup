// grid2.cpp

// version of grid2.click designed for running through cpp

// use like: gcc -E -P grid2.cpp | click

#include "grid-node-info.h"

rh :: ReadHandlerCaller(1) 

// protocol els
nb :: Neighbor(NBR_TIMEOUT, MAC_ADDR, GRID_IP)
h :: Hello(HELLO_PERIOD, HELLO_JITTER, MAC_ADDR, GRID_IP)

// device layer els
ps :: FromDevice(NET_DEVICE, 0)
q :: ToDevice(NET_DEVICE)

// linux ip layer els
linux :: Tun(TUN_DEVICE, GRID_IP, GRID_NETMASK)
  to_linux :: Queue -> linux

// hook it all up
ps -> Classifier(GRID_ETH_PROTO) -> [0] nb [0] -> q 

linux -> cl :: Classifier(GRID_IP_HEX, // ip for us
			  GRID_NET_HEX, // ip for Grid network
			  -) // the rest of the world
cl [0] -> to_linux
cl [1] -> GetIPAddress(16) -> [1] nb [1] -> check :: CheckIPHeader [0] -> to_linux
check [1] -> Discard
cl [2] -> SetIPAddress(GRID_GW) -> [1] nb // for grid gateway
h -> q 

