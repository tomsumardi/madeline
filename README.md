#Madeline

### Status: 
- C++ logging 
- working on ipps (c++)....

## -------------------------------------------------------------
## Pre-requisite through package manager 
* gnome C library 2.46.2 (glib2)
* C++ boost library 1.53.0 (boost-devel)

## Building and running...

### 1. How to build: Each components must be build separately. 

* mutil
```
cd <install-home>/madeline/components/etc/cpp/mutil/scripts
./refresh-automake.sh
cd <install-home>/madeline/components/etc/cpp/mutil
make clean
./configure
make
```
* ipps:
```
cd <install-home>/madeline/components/ipps/scripts
./refresh-automake.sh
cd <install-home>/madeline/components/ipps
make clean
./configure
make
```

* cs: 
* ms: 
* phs:
* ppp:

### 2. How to clean: Each components must be clean separately. 
* mutil
```
cd <install-home>/madeline/components/etc/cpp/mutil
make clean
```
* ipps:
```
cd <install-home>/madeline/components/ipps
make clean
```
* cs: 
* ms: 
* phs:
* ppp:

### 2. How to run: Each components must be run separately. 
* ipps: 
```
cd <install-home>/madeline/components/ipps
ipps -h
Usage:
  ipps [OPTION...] - IPPS

Help Options:
  -h, --help            Show help options

Application Options:
  -v, --verbose         verbose
  -s, --standalone      standalone, no MS
  -c, --config=file     json config file
```
* cs: 
* ms: 
* phs:
* ppp:

## -------------------------------------------------------------
## Project Intro and Vision:
Madeline is high performance OTT Video caching ecosystem, which includes simple REST API endpoint, packet routing engine and video caching server.
The vision is to have a baseline framework where the ecosystem can be used for more than caching video but also capturing and monitoring single L2/L3 packet flow or session monitoring/stiching, that can be analyzed in the future through some kind of storage backend such as ceph.

### Motivation:
Caching video online content on the ISP edges to provide better customer viewing experience. 

### Summary:
Madeline is out-of/in-band inline (realtime) transparent caching ecosystem, which performs intelligent OTT online video (MPEG-DASH/apple HLS/etc) caching based on regular expression or ip address/port tuple logic rulesets. 
It must be fast in order to perform intelligent routing/load-balancing and filtering of packets per flow pulled from linux socket kernel buffer (SKB) bypassing linux network stack. 
The packets are decoded and dissected from layer 2 to 4 while the payload is untouched and pushed from one service to another in pipelined manner. 
Filtering will be done on packet-processing and http processing components and in the end interesting packets will be redirected to the caching web server such nginx/httpd/etc.

Note: 
  - this is my second attempt, total rewrite the original application specific monolithic design
  - first pass will be done on out-of-band work
  - https (TLS) packets capturing with datastore/openstack on the backend (future project)
  - network analytics (future project) 

### components:
* Interface Packet Processing Service (IPPS)
* Packet Payload Processing (PPP)
* Packet Handling Service (PHS)
* Management Server (MS)
* Caching Server (CS)
* Simulation (SIM)

Note: per-component design details provided within design folder

### Resources:

* Design Document wiki pages: 
 - High level: https://github.com/tomsumardi/madeline/wiki
 - IPPS: https://github.com/tomsumardi/madeline/wiki/IPPS
 - PPP: https://github.com/tomsumardi/madeline/wiki/PPP
 - PHS: https://github.com/tomsumardi/madeline/wiki/PHS

* Test (anything non-https):
 - caching espn, amazon streaming, HBOgo, dailymotion, netflix (if possible), etc

* Development environment: Virtual Machine running CentOS 7.x.

* Lab Requirements:
 - any cheap L3 router/switch
 - any x86 laptop with 3 ethernet ports (mgmt,input,output)
 - Dualcomm model No.DCSW-1005 5-port 10/100Base-T Ethernet Switch TAP 

### My Old Code References:
* trans cache ("C" code): 
https://github.com/tomsumardi/laguna/tree/master/1.0/src
* gateway discovery (python):
https://github.com/tomsumardi/laguna/blob/master/1.0/scripts/rpm/transparent_caching_1.0.0/usr/local/bin/transparent_caching/gwdisc.py

Coding guidelines for C++:
- https://google.github.io/styleguide/cppguide.html
- No C++ exceptions handling, do simple C style error return value (read google guidelines for C++ above)
- shallow inheritance (3 level max)
- ?

license: TBD
