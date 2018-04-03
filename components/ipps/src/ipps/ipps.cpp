#include "ipps.h"

MSTS ipps::processCmdArgs(int argc, char *argv[])
{
    MSTS                _sts = MDERROR;
    GError*             _error = NULL;
    GOptionContext*     _context;
    const gchar*        _strHelpMenu = NULL;
    gboolean            _bVerbose = FALSE;
    gboolean            _bStandalone = FALSE;
    gchar               *_strIppsConfig = NULL;
    gchar               *_strIppsSchm = NULL;

    GOptionEntry entries[] =
    {
    //  { "var", 'x', 0, G_OPTION_ARG_INT, &variable, "variable", "N" },
      { "verbose", 'v', 0, G_OPTION_ARG_NONE, &_bVerbose, "verbose", NULL },
      { "standalone", 'a', 0, G_OPTION_ARG_NONE, &_bStandalone, "standalone, no MS", NULL },
      { "ippsconf", 'i', 0, G_OPTION_ARG_FILENAME, &_strIppsConfig, "ipps config", "file" },
      { "ippsschm", 's', 0, G_OPTION_ARG_FILENAME, &_strIppsSchm, "ipps schema", "file" },
      ARG_NONE
    };

    try
    {
          _context = g_option_context_new ("- IPPS");
          if(NULL == _context)
              throw_with_trace(runtime_error("option context create failure"));
          g_option_context_add_main_entries (_context, entries, NULL);
          if (!g_option_context_parse (_context, &argc, &argv, &_error))
          {
              IPPSLOG->error("option parsing failed: {}",_error->message);
              throw_with_trace(runtime_error(""));
          };
          _strHelpMenu = g_option_context_get_help(_context,TRUE, NULL);
          if(NULL == _strHelpMenu)
          {
              throw_with_trace(runtime_error("option get help failed"));
          }
          if(_bVerbose)
          {
              ipps::setVerbose(_bVerbose);
          }
          if(_bStandalone)
          {
              ipps::setStandalone(_bStandalone);
          }
          if(!_strIppsConfig)
          {
              throw_with_trace(runtime_error("error, you must specify -i"));
          }
          if(!_strIppsSchm)
          {
              throw_with_trace(runtime_error("error, you must specify -s"));
          }
          //use c++ rapidJson libary to parse json
          BOOST_ASSERT(ipps::getIppsJsonDoc());
          //TODO: dangerous passing pointer document here, will need to do
          //deep copy within the parser class and return document
          mutilMparse jsonParser(_strIppsConfig,&m_ippsDoc,
                  _strIppsSchm,&m_ippsSchema,IPPSLOG);
          _sts = jsonParser.processJson();
          if(MDSUCCESS != _sts)
          {
              IPPSLOG->error("failed to parse json file, {}",_strIppsConfig);
              throw_with_trace(runtime_error(""));
          }
          _sts = jsonParser.validate();
          if(MDSUCCESS != _sts)
          {
              IPPSLOG->error("failed to validate json file, {}",_strIppsConfig);
              throw_with_trace(runtime_error(""));
          }
          _sts = MDSUCCESS;
    }
    catch (const std::exception& e) { IPPS_STACKTRACE(e) }

    return _sts;
}

MSTS ipps::configureSysLog()
{
    MSTS                _sts = MDERROR;

    try
    {
        spdlog::level::level_enum   _lvl;

        const Value& _log = m_ippsDoc["log"];

        if(boost::iequals(_log["level"].GetString(), "trace"))
            _lvl = MD_LTRACE;
        else if(boost::iequals(_log["level"].GetString(), "debug"))
            _lvl = MD_LDEBUG;
        else if(boost::iequals(_log["level"].GetString(), "info"))
            _lvl = MD_LINFO;
        else if(boost::iequals(_log["level"].GetString(), "warn"))
            _lvl = MD_LWARN;
        else if(boost::iequals(_log["level"].GetString(), "error"))
            _lvl = MD_LERROR;
        else if(boost::iequals(_log["level"].GetString(), "crit"))
            _lvl = MD_LCRIT;
        else if(boost::iequals(_log["level"].GetString(), "off"))
            _lvl = MD_LOFF;
        else
        {
            IPPSLOG->error("invalid log level input: \"{}\" valid inputs: "\
                    "trace,debug,info,warn,error,crit",_log["level"].GetString());
            throw_with_trace(runtime_error(""));
        }
        //Set log level
        m_eLogLvl = _lvl;

        // destroy all shared log pointers and recreate.
        spdlog::drop_all();

        //Get directory path
        string _location(_log["dir_location"].GetString());
        //setup system log
        mlogging _ippsmlog(_location+"/"+"ipps", "log", _lvl);
        _sts = _ippsmlog.addRotate(_log["size_mb"].GetInt()*1024*1024,_log["num"].GetInt());
        if(MDSUCCESS != _sts)
            throw_with_trace(runtime_error(""));
        IPPSLOG = _ippsmlog.getRotateLog();
        _sts = MDSUCCESS;
    }catch (const std::exception& e) { IPPS_STACKTRACE(e) }

    return(_sts);
}

MSTS ipps::configureComChannels()
{
    return(MDSUCCESS);
}

MSTS ipps::configurePfring()
{
    MSTS            _sts = MDERROR;
    const Value&    _log = m_ippsDoc["log"];
    string          _location(_log["dir_location"].GetString());

    try
    {
        //setup rotating logging
        std::shared_ptr<mlogging> _pThdLogDoc( new mlogging("ppthd", "log", m_eLogLvl) );
        if(!_pThdLogDoc)
            throw_with_trace(runtime_error("error malloc logging object for pktproc threads"));
        _sts = _pThdLogDoc->addRotate(_log["size_mb"].GetInt()*1024*1024,_log["num"].GetInt());
        if(MDSUCCESS != _sts)
            throw_with_trace(runtime_error(""));
        m_vpThread.init(getVerbose(), _pThdLogDoc);

        //Use clustering and one thread reads individual or bonded interface
        shared_ptr<rxtxInterface> _pRxTx(new rxtxAdapter < pfringDPI >
                            (new pfringDPI(getVerbose(),m_vpThread.getThdLogHandler(),&m_ippsDoc),
                            &pfringDPI::getPktHeader));
        if(!_pRxTx)
        {
            _sts = MDERROR;
            throw_with_trace(runtime_error("failure rxtx object memory allocation"));
        }
        m_vpThread.addPfring(_pRxTx);
    }catch (const std::exception& e) { IPPS_STACKTRACE(e) }

    return(_sts);
}

MSTS ipps::configureFilters()
{
    return(MDSUCCESS);
}

MSTS ipps::threadExec()
{
    MSTS                             _sts       = MDERROR;
    boost::posix_time::time_duration _timeout =
            boost::posix_time::seconds(IPPS_THDJOIN_TIMEOUT_SEC);

    try
    {
        /* background thread scheduler */
        //1.Launch IPPS thread
        m_vpThread.start();
        boost::this_thread::sleep(boost::posix_time::seconds(1));

        //2.snchronization, wait until thread is ready to go.
        //check if the thread is ready to go
        //shared variables are mutex protected always...
        while(!m_vpThread.isInitReady())
            boost::this_thread::sleep(boost::posix_time::milliseconds(IPPS_SYNCH_TIMEOUT_MS));
        if(m_vpThread.isInitError())
            throw_with_trace(runtime_error("thread init error"));
        m_vpThread.interrupt();

        //3.wait until thread is done
        //synchronization, wait until all threads are ready to go.
        while(1)
        {
            if (m_vpThread.timedJoin(_timeout))
            {
                //finished
                IPPSLOG->info("Worker threads finished");
                break;
            }
            else
            {
                //Not finished;
                IPPSLOG->info("Worker threads not finished");
            }
            boost::this_thread::sleep(boost::posix_time::seconds(1));
        }
        _sts = MDSUCCESS;
    }catch (const std::exception& e) { IPPS_STACKTRACE(e) }

    return(_sts);
}

void ipps_print(const char *str)
{
    //cout << "ipps:" << str << endl;
}
