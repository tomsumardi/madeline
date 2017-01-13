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
              ipps::validateIppsJsonDocs();
          }
          if((_strIppsConfig && !_strSysConfig) ||
             (!_strIppsConfig && _strSysConfig))
          {
              pMIppsLog->error("error, you must specify -i and -s ,\
                                    if one argument is being specified");
          }

    }while(FALSE);

    return _sts;
}

void ipps::validateIppsJsonDocs()
{
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
    BOOST_ASSERT(log.HasMember("location"));
    BOOST_ASSERT(log["location"].IsString());
    BOOST_ASSERT(log.HasMember("size_mb"));
    BOOST_ASSERT(log["size_mb"].IsNumber());
    BOOST_ASSERT(log.HasMember("num"));
    BOOST_ASSERT(log["num"].IsNumber());
    BOOST_ASSERT(log.HasMember("level"));
    BOOST_ASSERT(log["level"].IsString());

}

MSTS ipps::configurelogs()
{
    MSTS                _sts = MDERROR;

    do
    {
        spdlog::level::level_enum   _lvl;
        string                      _strExt = "";
        string                      _strPath = "";
        vector<string>              _vStrLocation;

        const Value& ipps = systemDoc["ipps"];
        const Value& log = ipps["log"];

        if(boost::iequals(log["level"].GetString(), "trace"))
            _lvl = MD_LTRACE;
        else if(boost::iequals(log["level"].GetString(), "debug"))
            _lvl = MD_LDEBUG;
        else if(boost::iequals(log["level"].GetString(), "info"))
            _lvl = MD_LINFO;
        else if(boost::iequals(log["level"].GetString(), "warn"))
            _lvl = MD_LWARN;
        else if(boost::iequals(log["level"].GetString(), "error"))
            _lvl = MD_LERROR;
        else if(boost::iequals(log["level"].GetString(), "crit"))
            _lvl = MD_LCRIT;
        else if(boost::iequals(log["level"].GetString(), "off"))
            _lvl = MD_LOFF;
        else
        {
            pMIppsLog->error("invalid log level input: \"{}\" valid inputs: "\
                    "trace,debug,info,warn,error,crit",log["level"].GetString());
            break;
        }

        // destroy all shared log pointers and recreate.
        spdlog::drop_all();

        //Get path and extension
        string _location(log["location"].GetString());
        boost::split(_vStrLocation,_location,boost::is_any_of("."));
        if(_vStrLocation.size() > 1)
        {
            for (size_t _i = 0; _i < _vStrLocation.size()-1; _i++)
                _strPath += _vStrLocation[_i];
            _strExt = _vStrLocation[_vStrLocation.size()-1];
        }
        else
        {
            _strPath = _location;
            _strExt = "log";
        }
        mlogging _ippsmlog(_strPath, _strExt, _lvl);
        _sts = _ippsmlog.addRotate(log["size_mb"].GetInt()*1024*1024,log["num"].GetInt());
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
    return(MDSUCCESS);
}

void ipps_print(const char *str)
{
    //cout << "ipps:" << str << endl;
}
