#include "main.h"
//#include "test.h"

int main(int argc, char *argv[])
{
    MSTS            _sts = MDSUCCESS;
    ipps            _ipps;
    do
    {
        //+ parse main arguments
        _sts = mBkgrndProcessCmdArgs(argc,argv,&_ipps);
        if (_sts != MDSUCCESS)
            break;
        //+ configure debug logging (logging)
        _sts = mBkgrndConfigurelogs(&_ipps);
        if (_sts != MDSUCCESS)
            break;
        //+ register with MS if not standalone (main)
        //+ polls for new configuration from MS using AMQP channel (bkgrnd)
        _sts = mBkgrndConfigureComChannels(&_ipps);
        if (_sts != MDSUCCESS)
            break;
        //+ configure pfring (pfring)
        _sts = mBkgrndConfigurePfring(&_ipps);
        if (_sts != MDSUCCESS)
            break;
        //+ Configures L2/L3 Filters (pfilter)
        _sts = mBkgrndConfigureFilters(&_ipps);
        if (_sts != MDSUCCESS)
            break;
        //+ Spawning multiple worker threads based on configuration given (thdmgmt-pktproc)
        _sts = mBkgrndConfigureThds(&_ipps);
        if (_sts != MDSUCCESS)
            break;
    }while(FALSE);

    return _sts;
}

