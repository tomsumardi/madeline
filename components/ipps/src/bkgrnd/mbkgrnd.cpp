#include "mbkgrnd.h"

using namespace std;

//glib C input argument parsing
MSTS ippsBkgrnd::parse(int argc, char *argv[])
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
              g_print ("option context create failure");
              break;
          }
          g_option_context_add_main_entries (_context, entries, NULL);
          if (!g_option_context_parse (_context, &argc, &argv, &_error))
          {
              g_print ("option parsing failed: %s\n", _error->message);
              break;
          };
          _strHelpMenu = g_option_context_get_help(_context,TRUE, NULL);
          if(NULL == _strHelpMenu)
          {
              g_print ("option get help failed");
              break;
          }
          if(_strconfig != NULL)
          {
              //use c++ boost libary to parse json
              config.jsonObj.setJsonLoc(_strconfig);
              _sts = config.jsonObj.parse();
              if(_sts != MDSUCCESS)
              {
                  g_print ("failed to parse json input file");
                  break;
              }
          }
          if(_verbose)
          {
              config.verbose = _verbose;
          }
          if(_standalone)
          {
              config.standalone = _standalone;
          }
    }while(FALSE);

    return _sts;
}

void bkgrnd_print(const char *str)
{
    cout << "bkgrnd:" << str << endl;
}


