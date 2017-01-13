#ifndef _IPPS_H_
#define _IPPS_H_

#include "mutil/src/madeline.h"
#include "mutil/src/mutil.h"
#include "mpfring.h"
#include "mthreads.h"
#include <iostream>
#include <glib.h>
#include <boost/filesystem.hpp>

#define ARG_NONE { NULL,0,0,G_OPTION_ARG_NONE,NULL,NULL,NULL }

class ipps : public madeline{
  public:
    ipps(string path)
    {
        setVerbose(false);setStandalone(false);
        boost::filesystem::path dir(path);
        BOOST_ASSERT(boost::filesystem::create_directory(dir));
    };
    Document* getIppsJsonDoc() {return(&ippsDoc);}
    MSTS validateIppsJsonDocs();
    Document* getSysJsonDoc() {return(&systemDoc);}
    bool getStandalone() {return(bMstandalone);}
    void setStandalone(bool bStandalone) {bMstandalone = bStandalone;}
    bool getVerbose() {return(bMverbose);}
    void setVerbose(bool bVerbose) {bMverbose = bVerbose;}
    /* Configure */
    MSTS ProcessCmdArgs(int argc, char *argv[]);
    MSTS Configurelogs();
    MSTS ConfigureComChannels();
    MSTS ConfigurePfring();
    MSTS ConfigureFilters();
    MSTS ConfigureThds();
  private:
    Document    ippsDoc;
    Document    systemDoc;
    //ippsPfring  pring;
    //ippsThread  thread;
};

void ipps_print(const char *str);

#endif /* _IPPS_H_ */
