#ifndef _MADELINE_H_
#define _MADELINE_H_

#include <glib.h>
#include <boost/assert.hpp>
#include <boost/format.hpp>
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
#include "spdlog/spdlog.h"

using namespace std;
typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;
template <class E>
void throw_with_trace(const E& e) {
    throw boost::enable_error_info(e)
        << traced(boost::stacktrace::stacktrace());
}

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
