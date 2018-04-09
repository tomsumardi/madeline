#include "test.h"

#include "../ppp/ppp.h"
#include "src/pfilter/pfilter.h"
#include "src/thdmgmt/thdmgmt.h"

void print_all(void)
{
    ppp_print(" test libppp.a \n");
    thdmgmt_print(" test libthdmgmt.a \n");
    pfilter_print(" test libpfilter.a \n");
    logging_print(" test liblogging.a \n");
}
