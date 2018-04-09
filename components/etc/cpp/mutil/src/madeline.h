#ifndef _MADELINE_H_
#define _MADELINE_H_

#include <glib.h>
#include <boost/assert.hpp>
#include <boost/format.hpp>
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
#include "spdlog/spdlog.h"

#define M_IPPS_ERRNO_PARSE_CMD_ARG          100
#define M_IPPS_ERRNO_CONF_SYSLOG            101
#define M_IPPS_ERRNO_CONF_PFRING            102
#define M_IPPS_ERRNO_CONF_COM_CHANNELS      103
#define M_IPPS_ERRNO_THREAD_EXEC            104
#define M_IPPS_ERRNO_THREAD                 105

#define M_PPP_ERRNO_PARSE_CMD_ARG           120
#define M_PPP_ERRNO_CONF_SYSLOG             121
#define M_PPP_ERRNO_CONF_PFRING             122
#define M_PPP_ERRNO_CONF_COM_CHANNELS       123
#define M_PPP_ERRNO_THREAD_EXEC             124
#define M_PPP_ERRNO_THREAD                  125

using namespace std;
typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;
template <class E>
void throw_with_trace(const E& e) {
    throw boost::enable_error_info(e)
        << traced(boost::stacktrace::stacktrace());
}

#define DUMP_STACKTRACE_LOG(e, plog)          std::vector<string> stkTrace = stackTrace(e); \
        for (std::vector<string>::iterator it = stkTrace.begin(); it != stkTrace.end(); ++it) \
        {plog->error(*it);}

#define DUMP_STACKTRACE_CERR(e)          std::vector<string> stkTrace = stackTrace(e); \
        for (std::vector<string>::iterator it = stkTrace.begin(); it != stkTrace.end(); ++it) \
        {  cerr << *it << endl;  }

class madeline{
    public:
        madeline()
        {
            bMstandalone = false;
            bMverbose = false;
        }
        std::vector<string> stackTrace(const std::exception& e){
            std::vector<string> stkTrace;
            stkTrace.push_back(e.what());
            const boost::stacktrace::stacktrace* st = boost::get_error_info<traced>(e);
            if (st) {
                for (boost::stacktrace::frame frame: *st) {
                    stkTrace.push_back(boost::stacktrace::to_string(frame));
                }
            }
            return stkTrace;
        }
    protected:
        bool        bMstandalone;
        bool        bMverbose;
};

#define MDERROR -1
#define MDSUCCESS 0
typedef int  MSTS;

#endif /* _MADELINE_H_ */
