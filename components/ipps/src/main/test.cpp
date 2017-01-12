#include "test.h"

#include "src/ipps/ipps.h"
#include "src/pfilter/pfilter.h"
#include "src/thdmgmt/thdmgmt.h"

void print_all(void)
{
    ipps_print(" test libipps.a \n");
    thdmgmt_print(" test libthdmgmt.a \n");
    pfilter_print(" test libpfilter.a \n");
    logging_print(" test liblogging.a \n");
}
