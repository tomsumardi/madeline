#ifndef _MBKGRND_H_
#define _MBKGRND_H_

#include "src/ipps.h"
#include "src/bkgrnd/mparse.h"
#include "src/bkgrnd/mpfring.h"
#include "src/bkgrnd/mthreads.h"
#include "src/bkgrnd/mthreads.h"

/*

class ippsBkgrnd {
    ippsJson  json;
    ippsCmd   cmd;
    ippsPfring  pring;
    ippsThread  thread;
  public:
    logging()
};
*/

void bkgrnd_print(const char *str);
const gchar* bkgrndParseMainArgs(int argc, char *argv[], GOptionEntry* entries);

#endif /* _MBKGRND_H_ */
