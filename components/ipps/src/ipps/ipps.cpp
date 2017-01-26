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

    GOptionEntry entries[] =
    {
    //  { "var", 'x', 0, G_OPTION_ARG_INT, &variable, "variable", "N" },
      { "verbose", 'v', 0, G_OPTION_ARG_NONE, &_bVerbose, "verbose", NULL },
      { "standalone", 'a', 0, G_OPTION_ARG_NONE, &_bStandalone, "standalone, no MS", NULL },
      { "ippsconf", 'i', 0, G_OPTION_ARG_FILENAME, &_strIppsConfig, "ipps config", "file" },
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
              if(MDSUCCESS == ipps::validateIppsJsonDocs())
              {
                  const Value& _pfring = ippsDoc["pfring"];
                  if(_pfring.HasMember("threads"))
                      pfringConf.nThreads = _pfring["threads"].GetInt();
                  if(_pfring.HasMember("core_bind_id"))
                      pfringConf.nCoreBindId = _pfring["core_bind_id"].GetInt();
                  if(_pfring.HasMember("watermark"))
                      pfringConf.nWatermark = _pfring["watermark"].GetInt();
                  if(_pfring.HasMember("poll_wait_msec"))
                      pfringConf.nPollWaitMsec = _pfring["poll_wait_msec"].GetInt();
                  if(_pfring.HasMember("ring_cluster_id"))
                      pfringConf.nRingClusterId = _pfring["ring_cluster_id"].GetInt();
                  if(_pfring.HasMember("hw_timestamp"))
                  {
                      if(boost::iequals(_pfring["hw_timestamp"].GetString(),"yes"))
                          pfringConf.bHwTimestamp = true;
                      else
                          pfringConf.bHwTimestamp = false;
                  }
                  if(_pfring.HasMember("strip_timestamp"))
                  {
                      if(boost::iequals(_pfring["strip_timestamp"].GetString(),"yes"))
                          pfringConf.bStripTimestamp = true;
                      else
                          pfringConf.bStripTimestamp = false;
                  }
              }
              else
              {
                  pMIppsLog->error("error, you must specify -i ,\
                                        if one argument is being specified");
                  break;
              }
          }

    }while(FALSE);

    return _sts;
}

MSTS ipps::validateIppsJsonDocs()
{
    MSTS    _sts = MDERROR;
   
    pMIppsLog->debug("validating ipps config");
    // TODO: validate Document ippsDoc, more validation needed
    BOOST_ASSERT(ippsDoc.HasMember("version"));
    BOOST_ASSERT(ippsDoc["version"].IsString());
    //cout << boost::format("version = %s\n") %ippsDoc["version"].GetString();

    const Value& _log = ippsDoc["log"];
    BOOST_ASSERT(ippsDoc.HasMember("log"));
    BOOST_ASSERT(_log.HasMember("dir_location"));
    BOOST_ASSERT(_log["dir_location"].IsString());
    BOOST_ASSERT(_log.HasMember("size_mb"));
    BOOST_ASSERT(_log["size_mb"].IsNumber());
    BOOST_ASSERT(_log.HasMember("num"));
    BOOST_ASSERT(_log["num"].IsNumber());
    BOOST_ASSERT(_log.HasMember("level"));
    BOOST_ASSERT(_log["level"].IsString());

    Value& _intfListIn = ippsDoc["interfaces_in"];
    BOOST_ASSERT(ippsDoc.HasMember("interfaces_in"));
    BOOST_ASSERT(ippsDoc["interfaces_in"].IsArray());

    // rapidjson uses SizeType instead of size_t.
    // name and direction are required values
    for (SizeType i = 0; i < _intfListIn.Size(); i++)
    {
        const Value& _intfElm = _intfListIn[i];

        if(_intfElm.HasMember("name"))
        {
            BOOST_ASSERT(_intfElm["name"].IsString());
            //cout << boost::format("name = %s\n") %_intfElm["name"].GetString();
        }
        if(_intfElm.HasMember("direction"))
        {
            BOOST_ASSERT(_intfElm["direction"].IsString());
            //cout << boost::format("direction = %s\n") %_intfElm["direction"].GetString();
        }
    }

    Value& _intfListOut = ippsDoc["interfaces_out"];
    BOOST_ASSERT(ippsDoc.HasMember("interfaces_out"));
    BOOST_ASSERT(ippsDoc["interfaces_out"].IsArray());
    for (SizeType i = 0; i < _intfListOut.Size(); i++)
    {
        const Value& _intfElm = _intfListOut[i];

        if(_intfElm.HasMember("name"))
        {
            BOOST_ASSERT(_intfElm["name"].IsString());
            //cout << boost::format("name = %s\n") %_intfElm["name"].GetString();
        }
    }
    const Value& _pfring = ippsDoc["pfring"];
    BOOST_ASSERT(ippsDoc.HasMember("pfring"));
    if(_pfring.HasMember("threads"))
      BOOST_ASSERT(_pfring["threads"].IsNumber());
    if(_pfring.HasMember("core_bind_id"))
      BOOST_ASSERT(_pfring["core_bind_id"].IsNumber());
    if(_pfring.HasMember("watermark"))
      BOOST_ASSERT(_pfring["watermark"].IsNumber());
    if(_pfring.HasMember("poll_wait_msec"))
      BOOST_ASSERT(_pfring["poll_wait_msec"].IsNumber());
    if(_pfring.HasMember("ring_cluster_id"))
      BOOST_ASSERT(_pfring["ring_cluster_id"].IsNumber());
    if(_pfring.HasMember("hw_timestamp"))
      BOOST_ASSERT(_pfring["hw_timestamp"].IsString());
    if(_pfring.HasMember("strip_timestamp"))
      BOOST_ASSERT(_pfring["strip_timestamp"].IsString());

    _sts = MDSUCCESS;

   return(_sts);
}

MSTS ipps::configureSysLog()
{
    MSTS                _sts = MDERROR;

    do
    {
        spdlog::level::level_enum   _lvl;

        const Value& _log = ippsDoc["log"];

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
    //check https://github.com/ntop/PF_RING/blob/dev/userland/lib/pfring.h
    //check https://github.com/ntop/PF_RING/blob/dev/userland/examples/pcount.c
    return(MDSUCCESS);
}

MSTS ipps::configureFilters()
{
    return(MDSUCCESS);
}

MSTS ipps::configureThds()
{
     MSTS            _sts = MDERROR;
     const Value&    _log = ippsDoc["log"];
     string          _location(_log["dir_location"].GetString());
     int             _nThds = pfringConf.nThreads;

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
