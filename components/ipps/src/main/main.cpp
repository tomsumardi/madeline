#include "main.h"

//TODO:
// -add json.schema validator (argument option where to find validator) (arg -s?)
// -handle optional values in arguments

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
    //no "GO TO:", trying to avoid C++ exception and 1
    //entry and 1 exit point code.
    //use C style exception for code maintainability
    //and to avoid future nightmare debugging nested
    //exceptions or "GO TO:".
    try
    {
        //+ parse main arguments
        _sts = _ipps.processCmdArgs(argc,argv);
        if (_sts != MDSUCCESS)
            throw_with_trace(runtime_error("Err xxx - processCmdArgs"));
        //+ configure debug logging (logging)
        _sts = _ipps.configureSysLog();
        if (_sts != MDSUCCESS)
            throw_with_trace(runtime_error("Err xxx - configureSysLog"));
        //+ configure pfring (pfring)
        _sts = _ipps.configurePfring();
        if (_sts != MDSUCCESS)
            throw_with_trace(runtime_error("Err xxx - configurePfring"));
        //+ register with MS if not standalone (main)
        //+ polls for new configuration from MS using AMQP channel (ipps)
        _sts = _ipps.configureComChannels();
        if (_sts != MDSUCCESS)
            throw_with_trace(runtime_error("Err xxx - throw_with_trace"));
        //+ Configures L2/L3 Filters (pfilter)
        //_sts = _ipps.configureFilters();
        //if (_sts != MDSUCCESS)
        //    throw_with_trace(runtime_error("Err xxx - configureFilters"));
        //+ Launch all threads
        _sts = _ipps.threadExec();
        if (_sts != MDSUCCESS)
            throw_with_trace(runtime_error("Err xxx - threadExec"));
    }catch (const std::exception& e) { }

    return _sts;
}

