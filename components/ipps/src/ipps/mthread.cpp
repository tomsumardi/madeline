#include "ipps.h"
using namespace std;

void mthread :: pProcessing()
{;
    std::string strTid = boost::lexical_cast<std::string>(boost::this_thread::get_id());
    pThdLog->info("thread ID:"+strTid);
}

