#Madeline

### Status: high level design in progress

### Summary: (need work)
Madeline is out-of/in-band inline (realtime) transparent caching ecosystem, which performs intelligent OTT online video (MPEG-DASH/apple HLS/etc) caching based on regular expression or ip address/port tuple logic rulesets. 
It must be fast in order to perform intelligent routing/load-balancing and filtering of packets per flow pulled from linux socket kernel buffer (SKB) bypassing linux network stack. 
The packets are decoded and dissected from layer 2 to 4 while the payload is untouched and pushed from one service to another in pipelined manner. 
Filtering will be done on packet-processing and http processing components and in the end interesting packets will be redirected to the caching web server such nginx/httpd/etc.

Note: 
  - this is my second attempt, total rewrite the original messed up monolithic design
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

Note: per-component design details provided within src folder

* Design Document (own webpage in the future): 
 - High level: https://github.com/tomsumardi/madeline/blob/master/design/madeline.docx
 - IPPS: https://github.com/tomsumardi/madeline/blob/master/design/ipps.docx

* Test (anything non-https):
 - caching espn, amazon streaming, HBOgo, dailymotion, netflix (if possible), etc

* Lab Requirements:
 - any x86 laptop with 2 ethernet ports
 - Dualcomm model No.DCSW-1005 5-port 10/100Base-T Ethernet Switch TAP 

license: TBD

