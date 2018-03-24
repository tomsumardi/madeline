#include "main.h"

//TODO:
// -add json.schema validator (argument option where to find validator) (arg -s?)
// -handle optional values in arguments

int main(int argc, char *argv[])
{
    MSTS    _sts;
    ipps    _ipps(LOG_IPPS_DNAME,
                 LOG_IPPS_FNAME,
                 LOG_IPPS_FEXT,
                 LOG_IPPS_LVL,
                 LOG_IPPS_FSIZE,
                 LOG_IPPS_FNUM);

    try
    {
        //+ parse main arguments
        _sts = _ipps.processCmdArgs(argc,argv);
        if (_sts != MDSUCCESS)
            throw M_IPPS_ERRNO_0;
        //+ configure debug logging (logging)
        _sts = _ipps.configureSysLog();
        if (_sts != MDSUCCESS)
            throw M_IPPS_ERRNO_1;
        //+ configure pfring
        _sts = _ipps.configurePfring();
        if (_sts != MDSUCCESS)
            throw M_IPPS_ERRNO_2;
        //+ register with MS if not standalone (main)
        //+ polls for new configuration from MS using AMQP channel (ipps)
        _sts = _ipps.configureComChannels();
        if (_sts != MDSUCCESS)
            throw M_IPPS_ERRNO_3;
        //+ Configures L2/L3 Filters (pfilter)
        //_sts = _ipps.configureFilters();
        //if (_sts != MDSUCCESS)
        //    throw M_IPPS_ERRNO_4;
        //+ Launch all threads
        _sts = _ipps.threadExec();
        if (_sts != MDSUCCESS)
            throw M_IPPS_ERRNO_5;
        _sts = MDSUCCESS;
    }catch (int e){
        _sts = e;
    }

    return _sts;
}

