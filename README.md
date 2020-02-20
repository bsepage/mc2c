# mc2c
Leaky Controller: Cross-VM Memory Controller Covert Channel on Multi-Core Systems

# What
This repository contains an instance of a cross-core covert channel using the memory controller. This version requires root privileges to set the CPU affinity for both processes, and to read the /proc/self/pagemap file. It also requires knowledge of the platform's DRAM addressing function.

# How
In order to launch the covert channel, adjust the DRAM map (in "src/map.cpp: pa2da()") according to your platform. Other parameters can be modified is src/config.cpp.

Prepare the execution as follows:
- make
- ./bin/gettime

It is required to have the receiver and sender to start communicating at the same time. The "./bin/gettime" command gives a measure of time comprised between 0 and 99 seconds. In two separate terminals, launch the following commands:
- sudo taskset 0x1 ./bin/sender *n*
- sudo taskset 0x2 ./bin/receiver *n*

Where *n* is the time at which sender and receiver launch the covert channel. For instance, if "./bin/gettime" outputs 70 seconds, set <*n* to 90 seconds in order to give you 20 seconds to write both commands.

The output file is written by the receiver in the sim/ folder. It is **highly** recommended to perform post-processing on the data set in order to visualize data properly. Alternatively, you can execute the attack in a low noise environment (e.g. run level 1).

Note that the sender and receiver will need to find virtual pointers located in specific regions of the DRAM memory. Because of virtualization, this will occasionally fail.
