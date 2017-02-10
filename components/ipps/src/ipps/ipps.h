#ifndef _IPPS_H_
#define _IPPS_H_

#include "mutil/src/madeline.h"
#include "mutil/src/mutil.h"
#include "mpfring.h"
#include "mthread.h"
#include <iostream>
#include <glib.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#define ARG_NONE { NULL,0,0,G_OPTION_ARG_NONE,NULL,NULL,NULL }
#define IPPS_SYNCH_TIMEOUT_MS       500
#define IPPS_THDJOIN_TIMEOUT_SEC    5
#define IPPSLOG     pMIppsLog

class ipps : public madeline{
  public:

    ipps(string strDname,
         string strFname,
         string strFext,
         spdlog::level::level_enum eLvl,
         int ifsize,
         int ifnum) : eLogLvl(spdlog::level::trace)
    {
        try
        {
            /* stop here if constructor failed */
            boost::filesystem::path dir(strDname);
            boost::filesystem::create_directories(dir);
            //setup logging
            //hardcode location for systemwide log location for now
            mlogging _ippsmlog(strDname+"/"+strFname, strFext, eLvl);
            BOOST_ASSERT(MDSUCCESS == _ippsmlog.addRotate(ifsize,ifnum));
            //track the sys log
            pMIppsLog = _ippsmlog.getRotateLog();
            BOOST_ASSERT(pMIppsLog);
            pMIppsLog->flush_on(spdlog::level::trace);
        }
        catch (exception const& e)
        {
          cerr << e.what() << endl;
          BOOST_ASSERT(NULL);
        }
    };
    ~ipps()
    {
        for (std::vector<mthread*>::iterator ppThd = vpThreads.begin();
                ppThd != vpThreads.end(); ++ppThd)
        {delete(*ppThd);}
    }
    Document* getIppsJsonDoc() {return(&ippsDoc);}
    MSTS validateIppsJsonDocs();
    std::shared_ptr<spdlog::logger> getSysLogHandler(){return(pMIppsLog);}
    bool getStandalone() {return(bMstandalone);}
    void setStandalone(bool bStandalone) {bMstandalone = bStandalone;}
    bool getVerbose() {return(bMverbose);}
    void setVerbose(bool bVerbose) {bMverbose = bVerbose;}
    /* Configure */
    MSTS processCmdArgs(int argc, char *argv[]);
    MSTS configureSysLog();
    MSTS configureComChannels();
    MSTS configurePfring();
    MSTS configureFilters();
    MSTS configureThds();
    MSTS runThds();
  private:
    //logging
    std::shared_ptr<spdlog::logger>     pMIppsLog;
    Document                            ippsDoc;
    Document                            ippsSchema;
    spdlog::level::level_enum           eLogLvl;
    //thread mgmt
    vector<mthread*>                    vpThreads;

    //thdmgmt                             threads;
    //ippsPfring  pring;
    //ippsThread  thread;
};

void ipps_print(const char *str);

#endif /* _IPPS_H_ */
