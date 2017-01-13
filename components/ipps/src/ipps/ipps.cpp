#include "ipps.h"

MSTS ipps::ProcessCmdArgs(int argc, char *argv[])
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
              cout << boost::format("option context create failure\n");
              break;
          }
          g_option_context_add_main_entries (_context, entries, NULL);
          if (!g_option_context_parse (_context, &argc, &argv, &_error))
          {
              cout << boost::format("option parsing failed: %s\n") %_error->message;
              break;
          };
          _strHelpMenu = g_option_context_get_help(_context,TRUE, NULL);
          if(NULL == _strHelpMenu)
          {
              cout << boost::format("option get help failed");
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
              mutilMparse jsonParser(_strIppsConfig,ipps::getIppsJsonDoc());
              _sts = jsonParser.processJson();
              if(MDSUCCESS != _sts)
              {
                  cout << boost::format("failed to parse json file, %s\n") %_strIppsConfig;
                  break;
              }
          }
          if(_strSysConfig)
          {
              //use c++ rapidJson libary to parse json
              BOOST_ASSERT(ipps::getSysJsonDoc());
              mutilMparse jsonParser(_strSysConfig,ipps::getSysJsonDoc());
              _sts = jsonParser.processJson();
              if(MDSUCCESS != _sts)
              {
                  cout << boost::format("failed to parse json file, %s\n") %_strSysConfig;
                  break;
              }
          }
          if(_strIppsConfig && _strSysConfig)
          {
              _sts = ipps::validateIppsJsonDocs();
              if(MDSUCCESS != _sts)
              {
                  break;
              }
          }
          if((_strIppsConfig && !_strSysConfig) ||
             (!_strIppsConfig && _strSysConfig))
          {
              cout << boost::format("error, you must specify -i and -s ,\
                                    if one argument is being specified\n");
          }

    }while(FALSE);

    return _sts;
}

MSTS ipps::validateIppsJsonDocs()
{
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
    // TODO: validate Document sysDoc here

    return MDSUCCESS;
}

MSTS ipps::Configurelogs()
{
    do
    {
        mlogging _ippsmlog("/tmp/test", "log", MD_LDEBUG);
        _ippsmlog.addRotate(1024*1024,5);
        std::shared_ptr<spdlog::logger> cmLog = _ippsmlog.getRotateLog();
        cmLog->error("Some log message");
    }while(FALSE);

    return(MDSUCCESS);
}

MSTS ipps::ConfigureComChannels()
{
    return(MDSUCCESS);
}

MSTS ipps::ConfigurePfring()
{
    return(MDSUCCESS);
}

MSTS ipps::ConfigureFilters()
{
    return(MDSUCCESS);
}

MSTS ipps::ConfigureThds()
{
    return(MDSUCCESS);
}

void ipps_print(const char *str)
{
    cout << "ipps:" << str << endl;
}
