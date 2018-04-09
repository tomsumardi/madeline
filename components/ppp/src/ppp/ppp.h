#ifndef _PPP_H_
#define _PPP_H_

#include "mutil/src/madeline.h"
#include "mutil/src/mutil.h"
#include "rxtxal/src/rxtxal.h"
//#include "mpfring.h"
#include <iostream>
#include <execinfo.h>
#include <glib.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include "../ppp/mthread.h"

#define ARG_NONE { NULL,0,0,G_OPTION_ARG_NONE,NULL,NULL,NULL }
#define PPP_SYNCH_TIMEOUT_MS       500
#define PPP_THDJOIN_TIMEOUT_SEC    5
#define PPPLOG                     m_pMPppLog
#define PPP_STACKTRACE(e)          DUMP_STACKTRACE_LOG(e, PPPLOG)

class ppp : public madeline{
  public:

    ppp(string strDname,
         string strFname,
         string strFext,
         spdlog::level::level_enum eLvl,
         int ifsize,
         int ifnum) : m_eLogLvl(spdlog::level::trace)
    {
        try
        {
            /* stop here if constructor failed */
            boost::filesystem::path dir(strDname);
            boost::filesystem::create_directories(dir);
            //setup logging
            //hardcode location for systemwide log location for now
            mlogging _pppmlog(strDname+"/"+strFname, strFext, eLvl);
            BOOST_ASSERT(MDSUCCESS == _pppmlog.addRotate(ifsize,ifnum));
            //track the sys log
            m_pMPppLog = _pppmlog.getRotateLog();
            BOOST_ASSERT(m_pMPppLog);
            m_pMPppLog->flush_on(spdlog::level::trace);
        }
        catch (const std::exception& e) { PPP_STACKTRACE(e) }
    };
    ~ppp()
    {}
    Document* getPppJsonDoc() {return(&m_pppDoc);}
    MSTS validatePppJsonDocs();
    std::shared_ptr<spdlog::logger> getSysLogHandler(){return(m_pMPppLog);}
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
    MSTS threadExec();

  private:
    //logging
    std::shared_ptr<spdlog::logger>     m_pMPppLog;
    Document                            m_pppDoc;
    Document                            m_pppSchema;
    spdlog::level::level_enum           m_eLogLvl;
    std::shared_ptr<mlogging>           _pThdLogDoc;
    mthread                             m_vpThread;
};

void ppp_print(const char *str);

#endif /* _PPP_H_ */
