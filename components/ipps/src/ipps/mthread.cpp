#include "ipps.h"
#include <net/ethernet.h> /* the L2 protocols */
using namespace std;

#define IPPS_MTHREAD_SLEEP_SEC      1
#define IPPS_NO_ZC_BUFFER_LEN       9000

void mthread :: procPkts()
{
    int                  _pktSent;
    MSTS                 _sts;
    rxtxal_pkthdr*       _pktHdr;
    int32_t              _tzone = gmt_to_local(0);
    u_char*              _pBuffer = NULL;
    u_int                _bufSize = IPPS_NO_ZC_BUFFER_LEN;
    char                 _fieldName[] = "interfaces_in";
    vector<string>       _strl3blacklist;

    try
    {
        _sts = m_pRxTx->open(_bufSize);
        if(MDSUCCESS != _sts)
            throw_with_trace(runtime_error("failure pfring initialization"));
        else
        {
            // Get list of L3 blacklisted sites
            const Value&  _intfField = m_pRxTx->getConfigValue(_fieldName);
            if(_intfField.HasMember("l3_blacklists"))
            {
                string  _strIp = "";
                auto& _lstIps = _intfField["l3_blacklists"];
                for (SizeType _i = 0; _i < _lstIps.Size(); _i++)
                {
                    _strIp = _lstIps[_i].GetString();
                    _strl3blacklist.push_back(_strIp);
                }
                for(vector<string>::const_iterator i = _strl3blacklist.begin(); i != _strl3blacklist.end(); ++i) {
                    THDLOG->info(*i);
                }
            }
            else
                throw_with_trace(runtime_error("failure l3 blacklist sites"));
        }

        // use C calloc instead of new
        _pBuffer = (u_char*)calloc(sizeof(u_char),_bufSize);
        if(!_pBuffer)
        {
            THDLOG->error("insufficient memory: {}",_bufSize);
            throw_with_trace(runtime_error(""));
        }
        else
        {
            // C memset
            memset(_pBuffer,0,sizeof(u_char)*_bufSize);
            while(1)
            {
                if(m_pRxTx->read_rx(_pBuffer,_bufSize) > 0)
                {
                    _pktHdr = m_pRxTx->getPktHeader();
                    if(_pktHdr)
                    {
                        switch(_pktHdr->eth_type)
                        {
                            case MD_IPV4:
                                m_pRxTx->printPacket(_tzone, _pBuffer);
                                _pktSent = m_pRxTx->write_tx(_pBuffer,_pktHdr->caplen);
                                if(_pktSent < 0)
                                    THDLOG->info("pfring_send err, {}",_pktSent);
                                _sts = m_pRxTx->flush_tx();
                                if(MDSUCCESS != _sts)
                                    THDLOG->info("pfring_flush_tx_packets err, {}",_sts);
                                break;

                            case MD_IPV6:
                                //will this work??
                                m_pRxTx->printPacket(_tzone, _pBuffer);
                                break;

                            default:
                                break;
                        }
                    }
                }
                else
                {
                    if(m_pRxTx->isWaitForPacket() == 0)
                        sched_yield();
                }
            }
        }
    } catch (const std::exception& e) {
        IPPS_THDSTACKTRACE(e)
        exit(M_IPPS_ERRNO_THREAD);
    }
}

void mthread :: pProcessing()
{
    //Thread level initialization, try not to initialize from here
    //if possible!! but from background thread.
    //init()
    //Delete this sleep, for testing purpose
    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    mthread::setInitReady(true);
    //Waiting to be interrupted if no error otherwise exit
    if(!m_bInitError)
    {
        try{
            while(1)
                boost::this_thread::sleep(
                        boost::posix_time::seconds(IPPS_MTHREAD_SLEEP_SEC));
        }
        catch(boost::thread_interrupted const& )
        {
            //clean resources
            THDLOG->info("Worker thread interrupted, processing packets..");
            std::string threadId =
                    boost::lexical_cast<std::string>(boost::this_thread::get_id());
            THDLOG->info("thread ID:"+threadId+" pulling packets..");
            mthread::procPkts();
            //Delete this sleep, for testing purpose
            //boost::this_thread::sleep(boost::posix_time::seconds(5));
        }
    }
}


