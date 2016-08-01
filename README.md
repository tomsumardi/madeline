#Madeline

### Status: high level design in progress

### Summary: (need work)
Madeline is out-of/in-band inline transparent caching engine, which performs intelligent routing of packets pulled from linux socket kernel buffer (SKB) bypassing linux network stack. 
The packets are decoded and dissected from layer 2 to 4 while the payload is untouched and pushed from one service to another in pipelined manner. 
Filtering will be done on packet-processing and http processing components and in the end interesting packets will be redirected to the caching server,
which in this case running nginx.

Note: first pass will be done on out-of-band work.

### daemon service components:
* Config settings (python):
 - system
 - network
 - resource
 - logging
 - backend
 - L4 regex matching
 - L2-L3 matching
 - communication channel
 
* Packet processing (C):
 - pulling packets from SKB
 - ring buffer resource allocation
 - L2-L4 decoding
 - L2-L4 packet filtering and routing
 - communication channel

* HTTP processing (C++):
 - above L4 decoding
 - above L4 packet filtering and routing
 - session correlation
 - communication channel 

* packet injection (C++):
 - stop packet packet generation 
 - redirection packet generation
 - RAW packet vector injection
 - communication channel 

* Backend (C++):
 - analytics 
 - log collector
 - ?
 - communication channel 
 
* Caching server:
 - nginx 
 
* Simulation (python):
 - simulate transparent caching 

### technologies/libraries:
- pfring library 
- zeromq library 
- C Glib (GNU library)
- mysql 
- nginx
- python twisted
- C++ boost
- etc

### language:
- python
- C
- C++

### architecture ideology:
http://martinfowler.com/articles/microservices.html

