#include "test.h"
#include "src/bkgrnd/lib.h"
#include "src/thdmgmt/lib.h"
#include "src/pfilter/lib.h"
#include "src/logging/lib.h"

void print_all(void)
{
    bkgrnd_print(" test1 \n");
    thdmgmt_print(" test2 \n");
    pfilter_print(" test3 \n");
    logging_print(" test4 \n");
}
