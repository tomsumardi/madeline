#Madeline

### Status: high level design in progress

### Summary: (need work)
Madeline is out-of/in-band inline (realtime) transparent caching engine, which performs intelligent OTT online video (MPEG-DASH/apple HLS/etc) caching based on regular expression or ip address/port tuple logic rulesets. 
It must be fast in order to perform intelligent routing/load-balancing and filtering of packets per flow pulled from linux socket kernel buffer (SKB) bypassing linux network stack. 
The packets are decoded and dissected from layer 2 to 4 while the payload is untouched and pushed from one service to another in pipelined manner. 
Filtering will be done on packet-processing and http processing components and in the end interesting packets will be redirected to the caching web server such nginx/httpd/etc.

Note: 
  - this is my second attempt, total rewrite the original messed up monolithic design
  - first pass will be done on out-of-band work
  - https (TLS) packets capturing with ceph datastore on the backend (future project)
  - network analytics (future project) 

### daemon service components:
* Config settings (python):
 - pp json schema
 - http-proc json schema
 - pinj json schema
 - ?
 - communication channel
 
* Packet processing (C):
 - pulling packets from SKB
 - ring buffer resource allocation
 - L2-L4 matching
 - L2-L4 decoding
 - L2-L4 packet filtering and routing
 - L4 regex matching
 - communication channel

* HTTP processing:
 - above L4 decoding (session, application, etc)
 - above L4 packet filtering and routing
 - plugin with session correlation as default plugin 
 - communication channel 

* packet injection:
 - secret sauce
 - stop packet packet generation 
 - redirection packet generation
 - RAW packet vector injection
 - communication channel 

* Backend:
 - httpd REST api endpoint
 - analytics 
 - log collector
 - plugin to hadoop cluster
 - plugin to ceph object store
 - communication channel 
 
* Caching server:
 - nginx 
 
* Simulation (python):
 - simulate transparent caching 

* Test (anything non-https):
 - caching espn, amazon streaming, HBOgo, dailymotion, netflix (if possible), etc

* Lab Requirements:
 - any x86 laptop with 2 ethernet ports
 - Dualcomm model No.DCSW-1005 5-port 10/100Base-T Ethernet Switch TAP 

license: TBD

architecture philosophy: microservice 

