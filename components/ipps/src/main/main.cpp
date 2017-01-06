#include "main.h"
#include "test.h"
using namespace std;

int main(int argc, char *argv[])
{
    int             _sts = MDSUCCESS;
    const gchar*    _strHelpMenu;
    do
    {
        printf("------ipps------\n");
        printf("main()\n");
        //+parse input arguments  (main)
        // --verbose
        // --standalone
        _strHelpMenu = bkgrndParseMainArgs(argc,argv,entries);
        if(NULL == _strHelpMenu)
        {
            _sts = 1;
            break;
        }
        print_all();
        //+ register with MS if not standalone (main)
        //+ polls for new configuration from MS using AMQP channel (bkgrnd)
        //+ loads ipps json configuration (bkgrnd)
        ippsJson jsonObj(JSON_PARSE_LOC);
        _sts = jsonObj.parse();
        if (_sts != MDSUCCESS)
            break;
        //+ configure pfring (pfring)
        //+ configure debug logging (logging)
        //+ Configures L2/L3 Filters (pfilter)
        //+ Spawning multiple worker threads based on configuration given (thdmgmt-pktproc)
    }while(FALSE);

    return _sts;
}

