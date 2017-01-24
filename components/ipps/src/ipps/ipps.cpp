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
    gchar               *_strSysConfig = NULL;

    GOptionEntry entries[] =
    {
    //  { "var", 'x', 0, G_OPTION_ARG_INT, &variable, "variable", "N" },
      { "verbose", 'v', 0, G_OPTION_ARG_NONE, &_bVerbose, "verbose", NULL },
      { "standalone", 'a', 0, G_OPTION_ARG_NONE, &_bStandalone, "standalone, no MS", NULL },
      { "ippsconf", 'i', 0, G_OPTION_ARG_FILENAME, &_strIppsConfig, "ipps config", "file" },
      { "sysconf", 's', 0, G_OPTION_ARG_FILENAME, &_strSysConfig, "system config", "file" },
      ARG_NONE
    };

    do
    {
          _context = g_option_context_new ("- IPPS");
          if(NULL == _context)
          {
              pMIppsLog->error("option context create failure");
              break;
          }
          g_option_context_add_main_entries (_context, entries, NULL);
          if (!g_option_context_parse (_context, &argc, &argv, &_error))
          {
              pMIppsLog->error("option parsing failed: {}",_error->message);
              break;
          };
          _strHelpMenu = g_option_context_get_help(_context,TRUE, NULL);
          if(NULL == _strHelpMenu)
          {
              pMIppsLog->error("option get help failed");
              break;
          }
          if(_bVerbose)
          {
              ipps::setVerbose(_bVerbose);
          }
          if(_bStandalone)
          {
              ipps::setStandalone(_bStandalone);
          }

          if(_strIppsConfig)
          {
              //use c++ rapidJson libary to parse json
              BOOST_ASSERT(ipps::getIppsJsonDoc());
              mutilMparse jsonParser(_strIppsConfig,ipps::getIppsJsonDoc(),pMIppsLog);
              _sts = jsonParser.processJson();
              if(MDSUCCESS != _sts)
              {
                  pMIppsLog->error("failed to parse json file, {}",_strIppsConfig);
                  break;
              }
          }
          if(_strSysConfig)
          {
              //use c++ rapidJson libary to parse json
              BOOST_ASSERT(ipps::getSysJsonDoc());
              mutilMparse jsonParser(_strSysConfig,ipps::getSysJsonDoc(),pMIppsLog);
              _sts = jsonParser.processJson();
              if(MDSUCCESS != _sts)
              {
                  pMIppsLog->error("failed to parse json file, {}",_strSysConfig);
                  break;
              }
          }
          if(_strIppsConfig && _strSysConfig)
          {
              if(ipps::validateIppsJsonDocs())
                break;
          }
          if((_strIppsConfig && !_strSysConfig) ||
             (!_strIppsConfig && _strSysConfig))
          {
              pMIppsLog->error("error, you must specify -i and -s ,\
                                    if one argument is being specified");
              break;
          }

    }while(FALSE);

    return _sts;
}

MSTS ipps::validateIppsJsonDocs()
{
    MSTS    _sts = MDERROR;
   
    pMIppsLog->debug("validating ipps config");
    // TDOD: validate Document ippsDoc, more validation needed
    BOOST_ASSERT(ippsDoc.HasMember("version"));
    BOOST_ASSERT(ippsDoc["version"].IsString());
    //cout << boost::format("version = %s\n") %ippsDoc["version"].GetString();

    const Value& intfList = ippsDoc["interfaces"];
    BOOST_ASSERT(ippsDoc.HasMember("interfaces"));
    BOOST_ASSERT(ippsDoc["interfaces"].IsArray());

    // rapidjson uses SizeType instead of size_t.
    // name and direction are required values
    for (SizeType i = 0; i < intfList.Size(); i++)
    {
        const Value& intfElm = intfList[i];

        if(intfElm.HasMember("name"))
        {
            BOOST_ASSERT(intfElm["name"].IsString());
            //cout << boost::format("name = %s\n") %intfElm["name"].GetString();
        }
        if(intfElm.HasMember("direction"))
        {
            BOOST_ASSERT(intfElm["direction"].IsString());
            //cout << boost::format("direction = %s\n") %intfElm["direction"].GetString();
        }
    }

    pMIppsLog->debug("validating system config");

    // validate Document systemDoc here
    BOOST_ASSERT(systemDoc.HasMember("version"));
    BOOST_ASSERT(systemDoc["version"].IsString());

    BOOST_ASSERT(systemDoc.HasMember("ipps"));
    const Value& ipps = systemDoc["ipps"];

    BOOST_ASSERT(ipps.HasMember("pktproc_threads"));
    BOOST_ASSERT(ipps["pktproc_threads"].IsNumber());

    const Value& log = ipps["log"];
    BOOST_ASSERT(log.HasMember("dir_location"));
    BOOST_ASSERT(log["dir_location"].IsString());
    BOOST_ASSERT(log.HasMember("size_mb"));
    BOOST_ASSERT(log["size_mb"].IsNumber());
    BOOST_ASSERT(log.HasMember("num"));
    BOOST_ASSERT(log["num"].IsNumber());
    BOOST_ASSERT(log.HasMember("level"));
    BOOST_ASSERT(log["level"].IsString());

    //TODO: Validate input values here
    do
    {
        if(ipps["pktproc_threads"].GetInt() <= 0)
        {
            pMIppsLog->error("threads <= 0");
            break;
        } 
    }while(FALSE);

    _sts = MDSUCCESS;

   return(_sts);
}

MSTS ipps::configureSysLog()
{
    MSTS                _sts = MDERROR;

    do
    {
        spdlog::level::level_enum   _lvl;

        const Value& _ipps = systemDoc["ipps"];
        const Value& _log = _ipps["log"];

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
            pMIppsLog->error("invalid log level input: \"{}\" valid inputs: "\
                    "trace,debug,info,warn,error,crit",_log["level"].GetString());
            break;
        }
        //Set log level
        ipps::eLogLvl = _lvl;

        // destroy all shared log pointers and recreate.
        spdlog::drop_all();

        //Get directory path
        string _location(_log["dir_location"].GetString());
        //setup system log
        mlogging _ippsmlog(_location+"/"+"ipps", "log", _lvl);
        _sts = _ippsmlog.addRotate(_log["size_mb"].GetInt()*1024*1024,_log["num"].GetInt());
        if(MDSUCCESS != _sts)
            break;
        pMIppsLog = _ippsmlog.getRotateLog();
        _sts = MDSUCCESS;
    }while(FALSE);

    return(_sts);
}

MSTS ipps::configureComChannels()
{
    return(MDSUCCESS);
}

MSTS ipps::configurePfring()
{
    return(MDSUCCESS);
}

MSTS ipps::configureFilters()
{
    return(MDSUCCESS);
}

MSTS ipps::configureThds()
{
     MSTS            _sts = MDERROR;
     const Value&    _ipps = systemDoc["ipps"];
     int             _nThds = _ipps["pktproc_threads"].GetInt();
     const Value&    _log = _ipps["log"];
     string          _location(_log["dir_location"].GetString());

    //registers all threads. each thread has its own class context
    for(int _i = 0; _i < _nThds; _i++)
    {
        //setup rotating logging
        mlogging* _pThdLogDoc = new mlogging(_location+"/"+"ppthd_"+std::to_string(_i), "log", ipps::eLogLvl);
        if(!_pThdLogDoc)
        {
            pMIppsLog->error("error malloc logging object for pktproc threads");
            break;
        }
        _sts = _pThdLogDoc->addRotate(_log["size_mb"].GetInt()*1024*1024,_log["num"].GetInt());
        if(MDSUCCESS != _sts)
            break;
        //save all threads
        mthread* _ppThd = new mthread(getVerbose(),_pThdLogDoc);
        if(!_ppThd)
        {
            pMIppsLog->error("error malloc threads");
            break;
        }
        vThreads.push_back(_ppThd);
    }

    _sts = MDSUCCESS;
     
    return(_sts);
}

MSTS ipps::runThds()
{
    MSTS                             _sts       = MDERROR;
    bool                             _bWaitThds = true;
    boost::posix_time::time_duration _timeout =
            boost::posix_time::seconds(IPPS_THDJOIN_TIMEOUT_SEC);

    /* background thread scheduler */
    //1.Launch all threads
    std::vector<mthread*>::iterator ppthd;
    for (ppthd = vThreads.begin() ; ppthd != vThreads.end(); ++ppthd)
        (*ppthd)->start();
    boost::this_thread::sleep(boost::posix_time::seconds(1));

    //2.snchronization, wait until all threads are ready to go.
    for (ppthd = vThreads.begin() ; ppthd != vThreads.end(); ++ppthd)
    {
        //check if the thread is ready to go
        //shared variables are mutex protected always...
        while(!(*ppthd)->isInitReady())
            boost::this_thread::sleep(boost::posix_time::milliseconds(IPPS_SYNCH_TIMEOUT_MS));
        if((*ppthd)->isInitError())
        {
            _bWaitThds = false;
            break;
        }
        (*ppthd)->interrupt();
    }

    //3.wait until all threads done
    //synchronization, wait until all threads are ready to go.
    if(_bWaitThds)
    {
        for (ppthd = vThreads.begin() ; ppthd != vThreads.end(); ++ppthd)
        {
            while(1)
            {

                if ((*ppthd)->timedJoin(_timeout))
                {
                    //finished
                    pMIppsLog->info("Worker threads finished");
                    break;
                }
                else
                {
                    //Not finished;
                    pMIppsLog->info("Worker threads not finished");
                }
                boost::this_thread::sleep(boost::posix_time::seconds(1));
            }
        }
        _sts = MDSUCCESS;
    }
    else
        pMIppsLog->error("failure init one of the threads");

    return(_sts);
}

void ipps_print(const char *str)
{
    //cout << "ipps:" << str << endl;
}
