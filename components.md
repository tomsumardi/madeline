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
 - plugin to openstack? object store?
 - communication channel 
 
* Caching server:
 - nginx 
 
* Simulation (python):
 - simulate transparent caching 

