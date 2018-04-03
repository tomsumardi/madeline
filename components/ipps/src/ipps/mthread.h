#ifndef _MTHREADS_H_
#define _MTHREADS_H_

#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

using namespace std;

#define THDLOG                      m_pThdLog
#define IPPS_THDSTACKTRACE(e)       DUMP_STACKTRACE_LOG(e, THDLOG)

class mthread : public madeline {
    public:
        mthread() :
            m_bVerbose(false),
            m_bInitRdy(false),
            m_bInitError(false),
            m_pRxTx(NULL)
        {
        }
        void init(bool bverbose, std::shared_ptr<mlogging> pThdLogDoc)
        {
            m_bVerbose = bverbose;
            m_pThdLog = pThdLogDoc->getRotateLog();
        }
        std::shared_ptr<spdlog::logger> getThdLogHandler(){return(m_pThdLog);}
        void start(){m_tmThread = boost::thread(&mthread::pProcessing, this);}
        void join(){m_tmThread.join();}
        void interrupt(){m_tmThread.interrupt();}
        void addPfring(shared_ptr<rxtxInterface> pRxTx) {m_pRxTx = pRxTx;}
        bool timedJoin(boost::posix_time::time_duration timeout){return(m_tmThread.timed_join(timeout));}
        void utilPrintPacket(const struct pfring_pkthdr *h, const u_char *p, int32_t tzone);
        bool isInitError()
        {
            bool _bVal;

            m_mtx_bInitError.lock();
            _bVal = m_bInitError;
            m_mtx_bInitError.unlock();

            return(_bVal);
        }
        void setInitError(bool bVal)
        {
            m_mtx_bInitError.lock();
            m_bInitError = bVal;
            m_mtx_bInitError.unlock();
        }
        bool isInitReady()
        {
            bool _bVal;

            m_mtx_bInitRdy.lock();
            _bVal = m_bInitRdy;
            m_mtx_bInitRdy.unlock();

            return(_bVal);
        }
        void setInitReady(bool bVal)
        {
            m_mtx_bInitRdy.lock();
            m_bInitRdy = bVal;
            m_mtx_bInitRdy.unlock();
        }
        void pProcessing();
        void procPkts();
    private:
        string                          m_strTest;
        bool                            m_bVerbose;
        boost::thread                   m_tmThread;
        std::shared_ptr<mlogging>       m_pLogDoc;
        std::shared_ptr<spdlog::logger> m_pThdLog;
        boost::mutex                    m_mtx_bInitRdy;
        bool                            m_bInitRdy;
        boost::mutex                    m_mtx_bInitError;
        bool                            m_bInitError;
        shared_ptr<rxtxInterface>       m_pRxTx;
};

#endif /* _MTHREADS_H_ */
