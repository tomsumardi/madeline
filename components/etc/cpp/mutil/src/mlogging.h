#ifndef _LOGGINGX_H_
#define _LOGGINGX_H_

#include "madeline.h"
#include "spdlog/spdlog.h"

#define MD_LTRACE     spdlog::level::level_enum::trace
#define MD_LDEBUG     spdlog::level::level_enum::debug
#define MD_LINFO      spdlog::level::level_enum::info
#define MD_LWARN      spdlog::level::level_enum::warn
#define MD_LERROR     spdlog::level::level_enum::err
#define MD_LCRIT      spdlog::level::level_enum::critical
#define MD_LOFF       spdlog::level::level_enum::off

class mlogging{
    public:
        mlogging(string strlname, string strext, spdlog::level::level_enum lvl)
        {
            mStrlname = strlname;
            mStrext = strext;
            mloglvl = lvl;
        }
        MSTS addRotate(int ifsize, int ifrotnum);
        std::shared_ptr<spdlog::logger> getRotateLog(){return(mCombinedRotateLog);}
    private:
        string                      mStrlname;
        string                      mStrext;
        spdlog::level::level_enum   mloglvl;
        std::shared_ptr<spdlog::logger> mCombinedRotateLog;
        std::vector<spdlog::sink_ptr> mSinks;
};

void logging_print(const char *str);

#endif /* _LOGGINGX_H_ */
