#ifndef _MAIN_H_
#define _MAIN_H_

#include "../ppp/ppp.h"

//Default systemwide error value, which can be
//set to other level at runtime
#define LOG_PPP_LVL    MD_LERROR
#define LOG_PPP_DNAME  string("/tmp/ppp")
#define LOG_PPP_FNAME  string("ppp")
#define LOG_PPP_FEXT   string("log")
#define LOG_PPP_FSIZE  (10*1024*1024)
#define LOG_PPP_FNUM   5

#endif /* _MAIN_H_ */
