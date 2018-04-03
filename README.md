## Project Intro and Vision:
Madeline is high performance packet router/recorder. The vision is to have a baseline framework where the ecosystem can be used for more than caching video but also capturing and monitoring single L2/L3 packet flow or session monitoring/stiching, that can be analyzed in the future through some kind of object storage backend such as ceph.

### Summary:
Madeline is micro-service container friendly out-of/in-band inline (realtime) packet router/recorder. The packets are decoded and dissected from layer 2 to 4 while the payload is untouched and pushed from one service to another in pipelined manner. It also performs intelligent forwarding/routing/load-balancing and filtering of packets per flow pulled from linux socket kernel buffer (SKB) bypassing linux network stack or physical NIC. The architecture allows each of the packet processing components (IPPS, PPP and PHS) to act as standalone processes with the user free to chain them together as needed with "active" and "passive" mode of operations.
- In active mode, it can be used to perform OTT Video (MPEG-DASH/apple HLS/etc) caching based on regular expression or ip address/port tuple logic rulesets by injecting data into the packet payload. 
- In passive mode, it can be used to perform traffic recording using object store storage backend.

Note: 
  - first pass will be done on out-of-band work
  - https (TLS) packets capturing with datastore/openstack on the backend (future project)
  - network analytics (future project) 

### components:
* rx/tx abstraction layer (RXTXAL)
* Interface Packet Processing Service (IPPS)
* Packet Payload Processing (PPP)
* Packet Handling Service (PHS)

### Resources:

#### Design Document wiki pages: 
 - High level: https://github.com/tomsumardi/madeline/wiki/Architecture
 - RXTXAL: https://github.com/tomsumardi/madeline/wiki/RXTXAL
 - IPPS: https://github.com/tomsumardi/madeline/wiki/IPPS
 - PPP: https://github.com/tomsumardi/madeline/wiki/PPP
 - PHS: https://github.com/tomsumardi/madeline/wiki/PHS
 - JSON schema and examples: https://github.com/tomsumardi/madeline/wiki/JSON-schemas-and-examples

#### Test (anything non-https):
 - active mode caching espn, amazon streaming, HBOgo, dailymotion, etc
 - passive mode object storage cluster

#### Development environment: 
 - Virtual Machine running CentOS 7.x.

#### Lab Requirements:
 - any cheap L3 router/switch
 - any x86 laptop with 3 ethernet ports (mgmt,input,output)
 - Dualcomm model No.DCSW-1005 5-port 10/100Base-T Ethernet Switch TAP 

#### license: TBD

## -------------------------------------------------------------
## Pre-requisite through package manager
* gnome C library 2.46.2 (glib2)
* C++ boost library 1.66.0 (manually build) - needed for exception stack trace
* pfring x86_64          6.4.1-1122                     
* pfring-dkms            6.4.1-1122 
* pfring-drivers-zc-dkms 1.2-0
* kernel module pf_ring.ko

## Building and running...

### 1. How to build: Each components must be build separately. 

* mutil
```
cd <install-home>/madeline/components/etc/cpp/mutil
make clean
./configure
make
```
* rxtxal:
```
cd <install-home>/madeline/components/etc/cpp/rxtxal
make clean
./configure
make
```
* ipps:
```
cd <install-home>/madeline/components/ipps
make clean
./configure
make
```
* phs:
* ppp:

### 2. How to clean: Each components must be clean separately. 
* mutil
```
cd <install-home>/madeline/components/etc/cpp/mutil
make clean
```
* rxtxal:
```
cd <install-home>/madeline/components/rxtxal
make clean
```
* ipps:
```
cd <install-home>/madeline/components/ipps
make clean
```
* phs:
* ppp:

### 3. How to run: Each components must be run separately. 
* ipps: 
```
Usage:
  ipps [OPTION...] - IPPS

Help Options:
  -h, --help              Show help options

Application Options:
  -v, --verbose           verbose
  -a, --standalone        standalone, no MS
  -i, --ippsconf=file     ipps config
  -s, --ippsschm=file     ipps schema
  
 Example:
 [xxxx@centos7-dev ipps]$ sudo src/main/ipps -i ../cfg/examples/ipps.json -s ../cfg/schema/ipps.jschema
[2017-02-10 01:15:06.916] [/tmp/ipps/ppthd_0] [info] Worker thread interrupted, processing packets..
[2017-02-10 01:15:06.916] [/tmp/ipps/ppthd_0] [info] thread ID:7ff5d9ec8700 pulling packets..
[2017-02-10 01:15:10.839] [/tmp/ipps/ppthd_0] [info] 01:15:10.837638002 [RX][if_index=2][52:54:00:12:35:02 -> 08:00:27:84:99:B4] [IPv4][64.233.177.155:443 -> 10.0.2.15:37652] [l3_proto=TCP][hash=1256867967][tos=0][tcp_seq_num=1848960001] [caplen=60][len=60][parsed_header_len=0][eth_offset=0][l3_offset=14][l4_offset=34][payload_offset=58]
[2017-02-10 01:15:10.839] [/tmp/ipps/ppthd_0] [info] 01:15:10.838159047 [RX][if_index=2][52:54:00:12:35:02 -> 08:00:27:84:99:B4] [IPv4][64.233.177.155:443 -> 10.0.2.15:37652] [l3_proto=TCP][hash=1256867967][tos=0][tcp_seq_num=1848960002] [caplen=60][len=60][parsed_header_len=0][eth_offset=0][l3_offset=14][l4_offset=34][payload_offset=54]
[2017-02-10 01:15:10.865] [/tmp/ipps/ppthd_0] [info] 01:15:10.856716159 [RX][if_index=2][52:54:00:12:35:02 -> 08:00:27:84:99:B4] [IPv4][64.233.177.155:443 -> 10.0.2.15:37652] [l3_proto=TCP][hash=1256867967][tos=0][tcp_seq_num=1848960002] [caplen=2048][len=2894][parsed_header_len=0][eth_offset=0][l3_offset=14][l4_offset=34][payload_offset=54]
...
``` 
* phs:
* ppp:

### 4. build notes:
if unable to build, please do below:
```
cd <install-home>/madeline/components/ipps/scripts
./refresh-automake.sh
```

### Misc
#### My Old Code References:
* trans cache ("C" and Makefile code): 
https://github.com/tomsumardi/laguna/tree/master/1.0/src
* gateway discovery (python):
https://github.com/tomsumardi/laguna/blob/master/1.0/scripts/rpm/transparent_caching_1.0.0/usr/local/bin/transparent_caching/gwdisc.py

#### Coding guidelines for C++:
- https://google.github.io/styleguide/cppguide.html
- shallow inheritance (3 level max)
- function should have one entry and one exit point

#### How to compile C++ boost 1.66.0
(remove distro specific boost)
wget https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
Unzip
tar -xzf boost_1_66_0.tar.gz && cd boost_1_66_0
Bootstrap and Install
./bootstrap.sh --prefix=/usr
./b2 install --prefix=/usr --with=all
sudo ldconfig
sudo ldd src/main/ipps

#### TODO: 
1. IPPS
- kernel/user level filtering and blacklisting
- more logs, config assertions
- Adding throw, catch and stacktrace from boost 1.66.0

