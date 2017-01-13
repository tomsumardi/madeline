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

    do
    {
        //+ parse main arguments
        _sts = _ipps.ProcessCmdArgs(argc,argv);
        if (_sts != MDSUCCESS)
            break;
        //+ configure debug logging (logging)
        _sts = _ipps.Configurelogs();
        if (_sts != MDSUCCESS)
            break;
        //+ register with MS if not standalone (main)
        //+ polls for new configuration from MS using AMQP channel (ipps)
        _sts = _ipps.ConfigureComChannels();
        if (_sts != MDSUCCESS)
            break;
        //+ configure pfring (pfring)
        _sts = _ipps.ConfigurePfring();
        if (_sts != MDSUCCESS)
            break;
        //+ Configures L2/L3 Filters (pfilter)
        _sts = _ipps.ConfigureFilters();
        if (_sts != MDSUCCESS)
            break;
        //+ Spawning multiple worker threads based on configuration given (thdmgmt-pktproc)
        _sts = _ipps.ConfigureThds();
        if (_sts != MDSUCCESS)
            break;
    }while(FALSE);

    return _sts;
}

