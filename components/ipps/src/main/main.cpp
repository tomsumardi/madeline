#include "main.h"

//Default systemwide error value, which can be
//set to other level at runtime
#define LOG_IPPS_LVL    MD_LERROR
#define LOG_IPPS_FNAME  "/tmp/ipps/ipps"
#define LOG_IPPS_FSIZE  (10*1024*1024)
#define LOG_IPPS_FNUM   5

int main(int argc, char *argv[])
{
    MSTS                            _sts = MDSUCCESS;
    std::shared_ptr<spdlog::logger> _pMIppsLog;
    ipps                            _ipps(LOG_IPPS_FNAME);

    do
    {
        //+ setup logging 
        //hardcode loation for systemwide log location for now
        mlogging _ippsmlog(LOG_IPPS_FNAME, "log", LOG_IPPS_LVL);
        _sts = _ippsmlog.addRotate(LOG_IPPS_FSIZE,LOG_IPPS_FNUM);
        if(MDSUCCESS != _sts)
            break;
        //track the sys log
        _pMIppsLog = _ippsmlog.getRotateLog();
        _pMIppsLog->error("Some log message");
        //_ipps.setSysLog(_pMIppsLog);
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

