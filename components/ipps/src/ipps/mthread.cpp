#include "ipps.h"
#include <net/ethernet.h> /* the L2 protocols */
using namespace std;

void mthread :: procPkts()
{
    struct pfring_pkthdr _hdr;
    MSTS                 _sts;
    u_char *             _pbuffer = mthread::buffer;
    pfring*              _pdIn = pPfring->getPfringIngress();
    int32_t              _tzone = gmt_to_local(0);

    /* pin to a core, this is later if need be
    if((num_threads > 1) && (numCPU > 1))
    {
        if(bind2core(core_id) == 0)
          THDLOG->info("Set thread {} on core {}/{}", thread_id, core_id, numCPU);
    }
    */

    memset(&_hdr, 0, sizeof(_hdr));


    while(1)
    {
        if((_sts = pfring_recv(_pdIn, &_pbuffer,
                IPPS_NO_ZC_BUFFER_LEN, &_hdr, waitForPacket)) > 0)
        {
            switch(_hdr.extended_hdr.parsed_pkt.eth_type)
            {
                case MD_IPV4:
                    utilPrintPacket(&_hdr,_pbuffer,_tzone);
                    break;

                case MD_IPV6:
                    //will this work??
                    utilPrintPacket(&_hdr,_pbuffer,_tzone);
                    break;

                default:
                    break;
            }
        }
        else
        {
            if(waitForPacket == 0)
                sched_yield();
        }
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

//TODO: temporary helper, delete this code
void mthread::utilPrintPacket(const struct pfring_pkthdr *h, const u_char *p, int32_t tzone)
{
  int _s;
  u_int _usec, _nsec = 0;
  bool _extendedPktHdr = true;
  char _dumpStr[512] = { 0 };

 if(/*(!is_sysdig) &&*/ (h->ts.tv_sec == 0 || h->extended_hdr.parsed_pkt.offset.l3_offset == 0))
 {
    memset((void*)&h->extended_hdr.parsed_pkt, 0, sizeof(struct pkt_parsing_info));
    pfring_parse_pkt((u_char*)p, (struct pfring_pkthdr*)h, 5, 0, 1);
  }

 _s = (h->ts.tv_sec + tzone) % 86400;

  if(h->extended_hdr.timestamp_ns)
  {
    _s = ((h->extended_hdr.timestamp_ns / 1000000000) + tzone) % 86400;
    _usec = (h->extended_hdr.timestamp_ns / 1000) % 1000000;
    _nsec = h->extended_hdr.timestamp_ns % 1000;
  }
  else
  {
    _usec = h->ts.tv_usec;
  }

  snprintf(_dumpStr, sizeof(_dumpStr), "%02d:%02d:%02d.%06u%03u ",
          _s / 3600, (_s % 3600) / 60, _s % 60,
           _usec, _nsec);

  if(_extendedPktHdr)
  {
    char _bigbuf[4096];
    u_int _len;

    snprintf(&_dumpStr[strlen(_dumpStr)], sizeof(_dumpStr)-strlen(_dumpStr), "%s[if_index=%d]",
      h->extended_hdr.rx_direction ? "[RX]" : "[TX]",
      h->extended_hdr.if_index);

    pfring_print_parsed_pkt(_bigbuf, sizeof(_bigbuf), p, h);
    _len = strlen(_bigbuf);

    if(_len > 0) _bigbuf[_len-1] = '\0';

    snprintf(&_dumpStr[strlen(_dumpStr)], sizeof(_dumpStr)-strlen(_dumpStr), "%s", _bigbuf);
  }
  else
  {
    char _buf1[32], _buf2[32];
    struct ether_header *ehdr = (struct ether_header *) p;

    snprintf(&_dumpStr[strlen(_dumpStr)], sizeof(_dumpStr)-strlen(_dumpStr),
             "[%s -> %s][eth_type=0x%04X][caplen=%d][len=%d]",
             etheraddrString(ehdr->ether_shost, _buf1),
             etheraddrString(ehdr->ether_dhost, _buf2),
         ntohs(ehdr->ether_type),
         h->caplen, h->len);
  }
  THDLOG->info("{}",_dumpStr);
}

