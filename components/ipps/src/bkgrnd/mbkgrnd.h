#ifndef _MBKGRND_H_
#define _MBKGRND_H_

#include "src/ipps.h"
#include "src/bkgrnd/mparse.h"
#include "src/bkgrnd/mpfring.h"
#include "src/bkgrnd/mthreads.h"

#define ARG_NONE { NULL,0,0,G_OPTION_ARG_NONE,NULL,NULL,NULL }

class ippsBkgrnd {
  public:
    typedef struct config {
        ippsJson      jsonObj;
        bool          verbose;
        bool          standalone;
    }ippsConfig;
    ippsBkgrnd()
    {
        config.verbose = false;
        config.standalone = false;
    };
    MSTS parse(int argc, char *argv[]);
    ippsConfig* getConfig(){return(&config);}
  private:
    ippsConfig config;
    //ippsCmd   cmd;
    //ippsPfring  pring;
    //ippsThread  thread;
};

//#define JSON_PARSE_LOC "/home/tsumardi/madeline/components/cfg/examples/ipps.json"


void bkgrnd_print(const char *str);

#endif /* _MBKGRND_H_ */
