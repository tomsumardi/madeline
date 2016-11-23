#include "test.h"

#include "../bkgrnd/mbkgrnd.h"
#include "src/logging/logging.h"
#include "src/pfilter/pfilter.h"
#include "src/thdmgmt/thdmgmt.h"

void print_all(void)
{
    bkgrnd_print(" test1 \n");
    thdmgmt_print(" test2 \n");
    pfilter_print(" test3 \n");
    logging_print(" test4 \n");
}
