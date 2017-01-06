#include "test.h"

#include "../bkgrnd/mbkgrnd.h"
#include "src/logging/logging.h"
#include "src/pfilter/pfilter.h"
#include "src/thdmgmt/thdmgmt.h"

void print_all(void)
{
    bkgrnd_print(" test libbkgrnd.a \n");
    thdmgmt_print(" test libthdmgmt.a \n");
    pfilter_print(" test libpfilter.a \n");
    logging_print(" test liblogging.a \n");
}
