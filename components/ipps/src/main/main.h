#ifndef _MAIN_H_
#define _MAIN_H_

#include "src/ipps/ipps.h"

//Default systemwide error value, which can be
//set to other level at runtime
#define LOG_IPPS_LVL    MD_LTRACE
#define LOG_IPPS_DNAME  string("/tmp/ipps")
#define LOG_IPPS_FNAME  string("ipps")
#define LOG_IPPS_FEXT   string("log")
#define LOG_IPPS_FSIZE  (10*1024*1024)
#define LOG_IPPS_FNUM   5

#endif /* _MAIN_H_ */
