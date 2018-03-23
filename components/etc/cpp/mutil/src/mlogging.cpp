#include "mlogging.h"
using namespace std;

MSTS mlogging::addRotate(int ifsize, int ifrotnum)
{
    MSTS  _sts = MDSUCCESS;
    try
    {
        mSinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt> (mStrlname, mStrext, ifsize, ifrotnum));
        mSinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
        mCombinedRotateLog = std::make_shared<spdlog::logger>(mStrlname, begin(mSinks), end(mSinks));
        mCombinedRotateLog->set_level(mloglvl);
        spdlog::register_logger(mCombinedRotateLog);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        //std::cout << "Log initialization failed: " << ex.what() << std::endl;
        _sts = MDERROR;
    }

    return _sts;
}

void logging_print(const char *str)
{
    //cout << "logging:" << str << endl;
}
