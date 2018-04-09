#include "main.h"

int main(int argc, char *argv[])
{
    MSTS    _sts;
    ppp    _ppp(LOG_PPP_DNAME,
                 LOG_PPP_FNAME,
                 LOG_PPP_FEXT,
                 LOG_PPP_LVL,
                 LOG_PPP_FSIZE,
                 LOG_PPP_FNUM);

    try
    {
        //+ parse main arguments
        _sts = _ppp.processCmdArgs(argc,argv);
        if (_sts != MDSUCCESS)
            throw M_PPP_ERRNO_PARSE_CMD_ARG;
        //+ configure debug logging (logging)
        _sts = _ppp.configureSysLog();
        if (_sts != MDSUCCESS)
            throw M_PPP_ERRNO_CONF_SYSLOG;
        //+ configure pfring
        _sts = _ppp.configurePfring();
        if (_sts != MDSUCCESS)
            throw M_PPP_ERRNO_CONF_PFRING;
        //+ register with MS if not standalone (main)
        //+ polls for new configuration from MS using AMQP channel (ppp)
        _sts = _ppp.configureComChannels();
        if (_sts != MDSUCCESS)
            throw M_PPP_ERRNO_CONF_COM_CHANNELS;
        //+ Launch all threads
        _sts = _ppp.threadExec();
        if (_sts != MDSUCCESS)
            throw M_PPP_ERRNO_THREAD_EXEC;
        _sts = MDSUCCESS;
    }catch (int e){
        _sts = e;
    }

    return _sts;
}

