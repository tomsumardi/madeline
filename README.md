#Madeline

### Status: high level design in progress

### Summary: (need work)
Madeline is out-of/in-band inline (realtime) transparent caching engine, which performs intelligent OTT online video (Mpeg-DASH/apple HLS/etc) caching based on regular expression or ip address/port tuple logic rulesets. 
It must be fast in order to perform intelligent routing/load-balancing and filtering of packets per flow pulled from linux socket kernel buffer (SKB) bypassing linux network stack. 
The packets are decoded and dissected from layer 2 to 4 while the payload is untouched and pushed from one service to another in pipelined manner. 
Filtering will be done on packet-processing and http processing components and in the end interesting packets will be redirected to the caching web server such nginx/httpd/etc.

Note: 
  - this is my second attempt, rewritting the original messed up monolithic design
  - first pass will be done on out-of-band work.
  - network analytics (future project) 

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

* HTTP processing:
 - above L4 decoding
 - above L4 packet filtering and routing
 - session correlation
 - communication channel 

* packet injection:
 - stop packet packet generation 
 - redirection packet generation
 - RAW packet vector injection
 - communication channel 

* Backend:
 - analytics 
 - log collector
 - ?
 - communication channel 
 
* Caching server:
 - nginx 
 
* Simulation (python):
 - simulate transparent caching 

* Test (anything non-https):
 - caching espn, amazon streaming, HBOgo, dailymotion, netflix (if possible), etc

Requirements (garage project):
 - espn, amazon streaming, HBOgo, dailymotion, netflix account
 - regular x86 laptop
 - Dualcomm model No.DCSW-1005 5-port 10/100Base-T Ethernet Switch TAP 

license: TBD

#### Current work:
* config settings
  + new design based on old design mistakes




