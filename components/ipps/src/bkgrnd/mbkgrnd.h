#ifndef _MBKGRND_H_
#define _MBKGRND_H_

#include "src/ipps.h"
#include "src/bkgrnd/mparse.h"
#include "src/bkgrnd/mpfring.h"
#include "src/bkgrnd/mthreads.h"

#define ARG_NONE { NULL,0,0,G_OPTION_ARG_NONE,NULL,NULL,NULL }

MSTS mBkgrndProcessCmdArgs(int argc, char *argv[], ipps* pIpps);
MSTS mBkgrndConfigurelogs(ipps* pIpps);
MSTS mBkgrndConfigureComChannels(ipps* pIpps);
MSTS mBkgrndConfigurePfring(ipps* pIpps);
MSTS mBkgrndConfigureFilters(ipps* pIpps);
MSTS mBkgrndConfigureThds(ipps* pIpps);
void bkgrnd_print(const char *str);

#endif /* _MBKGRND_H_ */
