#ifndef _IPPS_H_
#define _IPPS_H_

#include "mutil/src/madeline.h"
#include "mutil/src/mutil.h"
#include "mpfring.h"
#include "mthreads.h"
#include <iostream>
#include <glib.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#define ARG_NONE { NULL,0,0,G_OPTION_ARG_NONE,NULL,NULL,NULL }

class ipps : public madeline{
  public:
    ipps(string strDname,
         string strFname,
         string strFext,
         spdlog::level::level_enum eLvl,
         int ifsize,
         int ifnum)
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
    Document* getIppsJsonDoc() {return(&ippsDoc);}
    void validateIppsJsonDocs();
    Document* getSysJsonDoc() {return(&systemDoc);}
    std::shared_ptr<spdlog::logger> getSysLogHandler(){return(pMIppsLog);}
    bool getStandalone() {return(bMstandalone);}
    void setStandalone(bool bStandalone) {bMstandalone = bStandalone;}
    bool getVerbose() {return(bMverbose);}
    void setVerbose(bool bVerbose) {bMverbose = bVerbose;}
    /* Configure */
    MSTS processCmdArgs(int argc, char *argv[]);
    MSTS configurelogs();
    MSTS configureComChannels();
    MSTS configurePfring();
    MSTS configureFilters();
    MSTS configureThds();
  private:
    std::shared_ptr<spdlog::logger>     pMIppsLog;
    Document                            ippsDoc;
    Document                            systemDoc;
    //ippsPfring  pring;
    //ippsThread  thread;
};

void ipps_print(const char *str);

#endif /* _IPPS_H_ */
