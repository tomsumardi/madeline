#include "main.h"
#include "test.h"
using namespace std;

int main(int argc, char *argv[])
{
    MSTS            _sts = MDSUCCESS;
    ippsBkgrnd      _bkgrnd;
    do
    {
        //+ parse main arguments
        _sts = _bkgrnd.parse(argc,argv);
        if (_sts != MDSUCCESS)
            break;
        //+ register with MS if not standalone (main)
        //+ polls for new configuration from MS using AMQP channel (bkgrnd)
        //+ loads ipps json configuration (bkgrnd)
        //+ configure pfring (pfring)
        //+ configure debug logging (logging)
        //+ Configures L2/L3 Filters (pfilter)
        //+ Spawning multiple worker threads based on configuration given (thdmgmt-pktproc)
    }while(FALSE);

    return _sts;
}

