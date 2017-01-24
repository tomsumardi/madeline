#ifndef _MTHREADS_H_
#define _MTHREADS_H_

#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

using namespace std;

class mthread{
    public:
        //mthread(bool bver, string strTestGlob) : bVerbose(bver) {strTest = strTestGlob;}
        mthread(bool bver,mlogging* pThdLogDoc)
        {
            pLogDoc = pThdLogDoc;
            pThdLog = pLogDoc->getRotateLog();
            bVerbose = bver;
        }
        ~mthread()
        {
            delete(pLogDoc);
        }
        void start(){tmThread = boost::thread(&mthread::pProcessing, this);}
        void join(){tmThread.join();}
        void pProcessing();
    private:
        string                          strTest;
        bool                            bVerbose;
        boost::thread                   tmThread;
        mlogging*                       pLogDoc;
        std::shared_ptr<spdlog::logger> pThdLog;
};

#endif /* _MTHREADS_H_ */
