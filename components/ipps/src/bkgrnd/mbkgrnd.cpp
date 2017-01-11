#include "mbkgrnd.h"
#include "spdlog/spdlog.h"

MSTS mBkgrndProcessCmdArgs(int argc, char *argv[],ipps* pIpps)
{
    MSTS                _sts = MDERROR;
    GError*             _error = NULL;
    GOptionContext*     _context;
    const gchar*        _strHelpMenu = NULL;
    gboolean            _verbose = FALSE;
    gboolean            _standalone = FALSE;
    gchar               *_strconfig = NULL;

    GOptionEntry entries[] =
    {
    //  { "var", 'x', 0, G_OPTION_ARG_INT, &variable, "variable", "N" },
      { "verbose", 'v', 0, G_OPTION_ARG_NONE, &_verbose, "verbose", NULL },
      { "standalone", 's', 0, G_OPTION_ARG_NONE, &_standalone, "standalone, no MS", NULL },
      { "config", 'c', 0, G_OPTION_ARG_FILENAME, &_strconfig, "json config file", "file" },
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
          if(_verbose)
          {
              pIpps->setVerbose(_verbose);
          }
          if(_standalone)
          {
              pIpps->setStandalone(_standalone);
          }
          if(_strconfig != NULL)
          {
              //use c++ rapidJson libary to parse json
              BOOST_ASSERT(pIpps->getJsonDoc());
              mutilMparse jsonParser(_strconfig,pIpps->getJsonDoc());
              _sts = jsonParser.read();
              if(_sts != MDSUCCESS)
              {
                  cout << boost::format("failed to read json input file");
                  break;
              }
              _sts = jsonParser.parse();
              if(_sts != MDSUCCESS)
              {
                  cout << boost::format("failed to parse json input file");
                  break;
              }
          }
    }while(FALSE);

    return _sts;
}

MSTS mBkgrndConfigurelogs(ipps* pIpps)
{
    return(MDSUCCESS);
}

MSTS mBkgrndConfigureComChannels(ipps* pIpps)
{
    return(MDSUCCESS);
}

MSTS mBkgrndConfigurePfring(ipps* pIpps)
{
    return(MDSUCCESS);
}

MSTS mBkgrndConfigureFilters(ipps* pIpps)
{
    return(MDSUCCESS);
}

MSTS mBkgrndConfigureThds(ipps* pIpps)
{
    return(MDSUCCESS);
}

void bkgrnd_print(const char *str)
{
    cout << "bkgrnd:" << str << endl;
}


