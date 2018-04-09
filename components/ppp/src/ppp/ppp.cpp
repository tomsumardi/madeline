#include "ppp.h"

MSTS ppp::processCmdArgs(int argc, char *argv[])
{
    MSTS                _sts = MDERROR;
    GError*             _error = NULL;
    GOptionContext*     _context;
    const gchar*        _strHelpMenu = NULL;
    gboolean            _bVerbose = FALSE;
    gboolean            _bStandalone = FALSE;
    gchar               *_strPppConfig = NULL;
    gchar               *_strPppSchm = NULL;

    GOptionEntry entries[] =
    {
    //  { "var", 'x', 0, G_OPTION_ARG_INT, &variable, "variable", "N" },
      { "verbose", 'v', 0, G_OPTION_ARG_NONE, &_bVerbose, "verbose", NULL },
      { "standalone", 'a', 0, G_OPTION_ARG_NONE, &_bStandalone, "standalone, no MS", NULL },
      { "pppconf", 'i', 0, G_OPTION_ARG_FILENAME, &_strPppConfig, "ppp config", "file" },
      { "pppschm", 's', 0, G_OPTION_ARG_FILENAME, &_strPppSchm, "ppp schema", "file" },
      ARG_NONE
    };

    try
    {
          _context = g_option_context_new ("- PPP");
          if(NULL == _context)
              throw_with_trace(runtime_error("option context create failure"));
          g_option_context_add_main_entries (_context, entries, NULL);
          if (!g_option_context_parse (_context, &argc, &argv, &_error))
          {
              PPPLOG->error("option parsing failed: {}",_error->message);
              throw_with_trace(runtime_error(""));
          };
          _strHelpMenu = g_option_context_get_help(_context,TRUE, NULL);
          if(NULL == _strHelpMenu)
          {
              throw_with_trace(runtime_error("option get help failed"));
          }
          if(_bVerbose)
          {
              ppp::setVerbose(_bVerbose);
          }
          if(_bStandalone)
          {
              ppp::setStandalone(_bStandalone);
          }
          if(!_strPppConfig)
          {
              throw_with_trace(runtime_error("error, you must specify -i"));
          }
          if(!_strPppSchm)
          {
              throw_with_trace(runtime_error("error, you must specify -s"));
          }
          //use c++ rapidJson libary to parse json
          BOOST_ASSERT(ppp::getPppJsonDoc());
          //TODO: dangerous passing pointer document here, will need to do
          //deep copy within the parser class and return document
          mutilMparse jsonParser(_strPppConfig,&m_pppDoc,
                  _strPppSchm,&m_pppSchema,PPPLOG);
          _sts = jsonParser.processJson();
          if(MDSUCCESS != _sts)
          {
              PPPLOG->error("failed to parse json file, {}",_strPppConfig);
              throw_with_trace(runtime_error(""));
          }
          _sts = jsonParser.validate();
          if(MDSUCCESS != _sts)
          {
              PPPLOG->error("failed to validate json file, {}",_strPppConfig);
              throw_with_trace(runtime_error(""));
          }
          _sts = MDSUCCESS;
    }
    catch (const std::exception& e) { PPP_STACKTRACE(e) }

    return _sts;
}

MSTS ppp::configureSysLog()
{
    MSTS                _sts = MDERROR;

    try
    {
        spdlog::level::level_enum   _lvl;

        const Value& _log = m_pppDoc["log"];

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
            PPPLOG->error("invalid log level input: \"{}\" valid inputs: "\
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
        mlogging _pppmlog(_location+"/"+"ppp", "log", _lvl);
        _sts = _pppmlog.addRotate(_log["size_mb"].GetInt()*1024*1024,_log["num"].GetInt());
        if(MDSUCCESS != _sts)
            throw_with_trace(runtime_error(""));
        PPPLOG = _pppmlog.getRotateLog();
        _sts = MDSUCCESS;
    }catch (const std::exception& e) { PPP_STACKTRACE(e) }

    return(_sts);
}

MSTS ppp::configureComChannels()
{
    return(MDSUCCESS);
}

MSTS ppp::configurePfring()
{
    MSTS            _sts = MDERROR;
    const Value&    _log = m_pppDoc["log"];
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
                            (new pfringDPI(getVerbose(),m_vpThread.getThdLogHandler(),&m_pppDoc),
                            &pfringDPI::getPktHeader));
        if(!_pRxTx)
        {
            _sts = MDERROR;
            throw_with_trace(runtime_error("failure rxtx object memory allocation"));
        }
        m_vpThread.addPfring(_pRxTx);
    }catch (const std::exception& e) { PPP_STACKTRACE(e) }

    return(_sts);
}

MSTS ppp::configureFilters()
{
    return(MDSUCCESS);
}

MSTS ppp::threadExec()
{
    MSTS                              _sts = MDSUCCESS;

    return(_sts);
}

void ppp_print(const char *str)
{
    //cout << "ppp:" << str << endl;
}
