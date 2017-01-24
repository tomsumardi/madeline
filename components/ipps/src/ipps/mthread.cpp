#include "ipps.h"
using namespace std;

void mthread :: pProcessing()
{
    //Thread level initialization, try not to initialize from here
    //if possible!! but from background thread.
    //init()
    //Delete this sleep, for testing purpose
    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    mthread::setInitReady(true);

    //Waiting to be interrupted if no error otherwise exit
    if(!mthread::bInitError)
    {
        try{
            while(1)
                boost::this_thread::sleep(
                        boost::posix_time::seconds(IPPS_MTHREAD_SLEEP_SEC));
        }
        catch(boost::thread_interrupted const& )
        {
            //clean resources
            pThdLog->info("Worker thread interrupted, processing packets..");
            std::string threadId =
                    boost::lexical_cast<std::string>(boost::this_thread::get_id());
            while(1)
            {
                pThdLog->info("thread ID:"+threadId+" pulling packets..");
                //Delete this sleep, for testing purpose
                boost::this_thread::sleep(boost::posix_time::seconds(5));
            }
        }
    }
}

