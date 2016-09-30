#ifndef _BKGRND_H_
#define _BKGRND_H_

#include "src/ipps.h"

void bkgrnd_print(const char *str);
const gchar* bkgrndParseMainArgs(int argc, char *argv[], GOptionEntry* entries);

#endif /* _BKGRND_H_ */
