#ifndef _MTHREADS_H_
#define _MTHREADS_H_

#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

using namespace std;

#define IPPS_MTHREAD_SLEEP_SEC      1

class mthread{
    public:
        mthread(bool bver,mlogging* pThdLogDoc) : bVerbose(bver), bInitRdy(false),bInitError(false)
        {
            pLogDoc = pThdLogDoc;
            pThdLog = pLogDoc->getRotateLog();
        }
        ~mthread()
        {
            delete(pLogDoc);
        }
        void start(){tmThread = boost::thread(&mthread::pProcessing, this);}
        void join(){tmThread.join();}
        void interrupt(){tmThread.interrupt();}
        void addPfring(mpfring* pRing) {pPfring = pRing;}
        bool timedJoin(boost::posix_time::time_duration timeout){return(tmThread.timed_join(timeout));}
        bool isInitError()
        {
            bool _bVal;

            mtx_bInitError.lock();
            _bVal = bInitError;
            mtx_bInitError.unlock();

            return(_bVal);
        }
        void setInitError(bool bVal)
        {
            mtx_bInitError.lock();
            bInitError = bVal;
            mtx_bInitError.unlock();
        }
        bool isInitReady()
        {
            bool _bVal;

            mtx_bInitRdy.lock();
            _bVal = bInitRdy;
            mtx_bInitRdy.unlock();

            return(_bVal);
        }
        void setInitReady(bool bVal)
        {
            mtx_bInitRdy.lock();
            bInitRdy = bVal;
            mtx_bInitRdy.unlock();
        }
        void pProcessing();
    private:
        string                          strTest;
        bool                            bVerbose;
        boost::thread                   tmThread;
        mlogging*                       pLogDoc;
        std::shared_ptr<spdlog::logger> pThdLog;
        boost::mutex                    mtx_bInitRdy;
        bool                            bInitRdy;
        boost::mutex                    mtx_bInitError;
        bool                            bInitError;
        mpfring*                        pPfring;
};

#endif /* _MTHREADS_H_ */
