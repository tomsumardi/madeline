#include "bkgrnd.h"
using namespace std;

const gchar* bkgrndParseMainArgs(int argc, char *argv[], GOptionEntry* entries)
{
	GError*             _error = NULL;
	GOptionContext*     _context;
    const gchar*        _strHelpMenu = NULL;

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
	}while(FALSE);

	return _strHelpMenu;
}

void bkgrnd_print(const char *str)
{
    cout << "bkgrnd:" << str << endl;
}


