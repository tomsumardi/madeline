#ifndef _MAIN_H_
#define _MAIN_H_

#include "src/ipps.h"
#include "src/bkgrnd/bkgrnd.h"

#define ARG_NONE { NULL,0,0,G_OPTION_ARG_NONE,NULL,NULL,NULL }

//static gint variable = 2;
static gboolean verbose = FALSE;
static gboolean standalone = FALSE;

static GOptionEntry entries[] =
{
//  { "var", 'x', 0, G_OPTION_ARG_INT, &variable, "variable", "N" },
  { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "verbose", NULL },
  { "standalone", 's', 0, G_OPTION_ARG_NONE, &standalone, "standalone, no MS", NULL },
  ARG_NONE
};


#endif /* _MAIN_H_ */
