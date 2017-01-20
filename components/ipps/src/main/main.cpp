#include "main.h"

int main(int argc, char *argv[])
{
    MSTS    _sts = MDSUCCESS;
    ipps    _ipps(LOG_IPPS_DNAME,
                 LOG_IPPS_FNAME,
                 LOG_IPPS_FEXT,
                 LOG_IPPS_LVL,
                 LOG_IPPS_FSIZE,
                 LOG_IPPS_FNUM);

    //RULES:
    //no "GO TO:", no C++ exception and 1
    //entry and 1 exit point code.
    //use C style exception for code maintainability
    //and to avoid future nightmare debugging nested
    //exceptions or "GO TO:".
    do
    {
        //+ parse main arguments
        _sts = _ipps.processCmdArgs(argc,argv);
        if (_sts != MDSUCCESS)
            break;
        //+ configure debug logging (logging)
        _sts = _ipps.configureSysLog();
        if (_sts != MDSUCCESS)
            break;
        //+ register with MS if not standalone (main)
        //+ polls for new configuration from MS using AMQP channel (ipps)
        _sts = _ipps.configureComChannels();
        if (_sts != MDSUCCESS)
            break;
        //+ configure pfring (pfring)
        _sts = _ipps.configurePfring();
        if (_sts != MDSUCCESS)
            break;
        //+ Configures L2/L3 Filters (pfilter)
        _sts = _ipps.configureFilters();
        if (_sts != MDSUCCESS)
            break;
        //+ Spawning multiple worker threads based on configuration given (thdmgmt-pktproc)
        _sts = _ipps.configureThds();
        if (_sts != MDSUCCESS)
            break;
    }while(FALSE);

    return _sts;
}

