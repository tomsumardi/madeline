#include "dpi_pfring.h"
#include <net/ethernet.h> /* the L2 protocols */

//Default settings ingress
#define RXTXAL_MPFRING_IN_BINDID             1
#define RXTXAL_MPFRING_IN_WATERMARK          10
#define RXTXAL_MPFRING_IN_POLLWAIT_MSEC      100
#define RXTXAL_MPFRING_IN_RING_CLUSTERID     55
#define RXTXAL_MPFRING_IN_HW_TIMESTAMP       false
#define RXTXAL_MPFRING_IN_STRIP_TIMESTAMP    true
#define RXTXAL_MPFRING_IN_SNAPLENGTH         2048
#define RXTXAL_MPFRING_IN_DIRECTION          "rx"

//Default settings egress
#define RXTXAL_MPFRING_OUT_BINDID             2
#define RXTXAL_MPFRING_OUT_WATERMARK          100
#define RXTXAL_MPFRING_OUT_POLLWAIT_MSEC      10
#define RXTXAL_MPFRING_OUT_RING_CLUSTERID     65
#define RXTXAL_MPFRING_OUT_HW_TIMESTAMP       false
#define RXTXAL_MPFRING_OUT_STRIP_TIMESTAMP    true
#define RXTXAL_MPFRING_OUT_SNAPLENGTH         2048
#define RXTXAL_MPFRING_OUT_DIRECTION          "tx"

// Default operations
MSTS pfringDPI::open(u_int bufSize)
{
  m_pLog->info("pfringDPI::open()");

  MSTS    _sts                = MDERROR;
  int     _nThreads           = 0;
  u_char  _mac_address[6]     = { 0 };
  char    _buf[32];

  do
  {
      _nThreads = m_ippsDoc["threads"].GetInt();
      m_pLog->debug("threads: {}",_nThreads);
      // default flow to 5 tuple for rx for now
      cluster_type cluster_hash_type = cluster_per_flow_5_tuple;

      if(m_ippsDoc.HasMember("interfaces_in"))
      {
          MSTS    _intfSts;
          string  _strDirection       = RXTXAL_MPFRING_IN_DIRECTION;
          int     _nBindId            = RXTXAL_MPFRING_IN_BINDID;
          int     _nWatermark         = RXTXAL_MPFRING_IN_WATERMARK;
          int     _nPollWaitMsec      = RXTXAL_MPFRING_IN_POLLWAIT_MSEC;
          int     _nRingClusterId     = RXTXAL_MPFRING_IN_RING_CLUSTERID;
          bool    _bHwTimeStamp       = RXTXAL_MPFRING_IN_HW_TIMESTAMP;
          bool    _bStripTimeStamp    = RXTXAL_MPFRING_IN_STRIP_TIMESTAMP;
          int     _nSnapLength        = RXTXAL_MPFRING_IN_SNAPLENGTH;
          u_int32_t _flags            = 0;

          const Value& _intfField = m_ippsDoc["interfaces_in"];
          if(_intfField.HasMember("direction"))
              _strDirection = _intfField["direction"].GetString();
          if(_intfField.HasMember("core_bind_id"))
              _nBindId = _intfField["core_bind_id"].GetInt();
          if(_intfField.HasMember("watermark"))
              _nWatermark = _intfField["watermark"].GetInt();
          if(_intfField.HasMember("poll_wait_msec"))
              _nPollWaitMsec = _intfField["poll_wait_msec"].GetInt();
          if(_intfField.HasMember("ring_cluster_id"))
              _nRingClusterId = _intfField["ring_cluster_id"].GetInt();
          if(_intfField.HasMember("hw_timestamp"))
          {
              if(boost::iequals(_intfField["hw_timestamp"].GetString(),"yes"))
                  _bHwTimeStamp = true;
              else
                  _bHwTimeStamp = false;
          }
          if(_intfField.HasMember("strip_timestamp"))
          {
              if(boost::iequals(_intfField["strip_timestamp"].GetString(),"yes"))
                  _bStripTimeStamp = true;
              else
                  _bHwTimeStamp = false;
          }

          string _strIntfName = "";
          auto&  _lstNames = _intfField["name"];
          for (SizeType _i = 0; _i < _lstNames.Size(); _i++)
          {
              _strIntfName += _lstNames[_i].GetString();
              if (_i < _lstNames.Size()-1)
                  _strIntfName += ",";
          }

          string  _strPcapFilter = "";
          auto& _lstFilters = _intfField["pcap_filters"];
          for (SizeType _i = 0; _i < _lstFilters.Size(); _i++)
          {
              _strPcapFilter += _lstFilters[_i].GetString();
              if (_i < _lstFilters.Size()-1)
                  _strPcapFilter += " || ";
          }

          m_pLog->debug("interfaces_in/snaplen: {}",_nSnapLength);
          m_pLog->debug("interfaces_in/direction: {}",_strDirection);
          m_pLog->debug("interfaces_in/core_bind_id: {}",_nBindId);
          m_pLog->debug("interfaces_in/watermark: {}",_nWatermark);
          m_pLog->debug("interfaces_in/poll_wait_msec: {}",_nPollWaitMsec);
          m_pLog->debug("interfaces_in/ring_cluster_id: {}",_nRingClusterId);
          m_pLog->debug("interfaces_in/hw_timestamp: {}",_bHwTimeStamp);
          m_pLog->debug("interfaces_in/strip_timestamp: {}",_bStripTimeStamp);
          m_pLog->debug("interfaces_in/name: {}",_strIntfName);
          m_pLog->debug("interfaces_in/pcap_filters: {}",_strPcapFilter);

          if(_nThreads > 1)     _flags |= PF_RING_REENTRANT;
          if(_bHwTimeStamp)     _flags |= PF_RING_HW_TIMESTAMP;
          if(_bStripTimeStamp)  _flags |= PF_RING_STRIP_HW_TIMESTAMP;
          _flags |= PF_RING_LONG_HEADER | PF_RING_PROMISC;

          /* common from here */
          m_pdIn = pfring_open(_strIntfName.c_str(), _nSnapLength, _flags);
          if(!m_pdIn)
          {
              m_pLog->error("failure to perform pfring_open on intf {}",_strIntfName);
              break;
          }
          u_int32_t _version;

          pfring_set_application_name(m_pdIn,(char*)"ipps");
          pfring_version(m_pdIn, &_version);

          m_pLog->info("Using PF_RING v.{}.{}.{}",
             (_version & 0xFFFF0000) >> 16,
             (_version & 0x0000FF00) >> 8,
             _version & 0x000000FF);

          if(pfring_get_bound_device_address(m_pdIn, _mac_address) != 0)
              m_pLog->warn("Unable to read the device address");
          else
          {
              int _ifindex = -1;

              pfring_get_bound_device_ifindex(m_pdIn, &_ifindex);

              m_pLog->info("Capturing from {} [mac: {}][if_index: {}][speed: {}Mb/s]",
                      (char*)_strIntfName.c_str(), etheraddrString(_mac_address, _buf),
                 _ifindex,
                     pfring_get_interface_speed(m_pdIn));
          }

          m_pLog->info("# Device RX channels: {}", pfring_get_num_rx_channels(m_pdIn));
          m_pLog->info("# Polling threads:    {}", _nThreads);

          /* setup bpf filtering, change this to pfring one in the future */
          _intfSts = pfring_set_bpf_filter(m_pdIn, (char*)(_strPcapFilter.c_str()));
          if(_intfSts != MDSUCCESS)
          {
              m_pLog->error("pfring_set_bpf_filter({}) returned {}", _strPcapFilter.c_str(), _intfSts);
              break;
          }
          else
              m_pLog->info("Successfully set BPF filter '{}'", _strPcapFilter.c_str());

          packet_direction direction;
          if(boost::iequals(_strDirection, "rx"))
              direction = rx_only_direction;
          else if(boost::iequals(_strDirection, "tx"))
              direction = tx_only_direction;
          else if(boost::iequals(_strDirection, "rx+tx"))
              direction = rx_and_tx_direction;
          else
          {
              m_pLog->error("pfring_set_direction, options are rx,tx and rx+tx");
              break;
          }
          pfring_set_direction(m_pdIn, direction);
          _intfSts = pfring_set_socket_mode(m_pdIn, recv_only_mode);
          if(MDSUCCESS != _intfSts)
          {
              m_pLog->error("pfring_set_socket_mode returned [_intfSts={}]", _intfSts);
              break;
          }

          if(_nWatermark > 0)
          {
            _intfSts = pfring_set_poll_watermark(m_pdIn, _nWatermark);
            if(MDSUCCESS != _intfSts)
            {
                m_pLog->error("pfring_set_poll_watermark returned [rc={}][watermark={}]", _intfSts, _nWatermark);
                break;
            }
          }
          else
          {
              m_pLog->error("pfring_set_poll_watermark must be > 0");
              break;
          }
          if(_nRingClusterId > 0)
          {
              _intfSts = pfring_set_cluster(m_pdIn, _nRingClusterId, cluster_hash_type);
              if(MDSUCCESS != _intfSts)
              {
                  m_pLog->error("pfring_set_cluster returned {}", _intfSts);
              }
          }
          if(_nPollWaitMsec > 0)
              pfring_set_poll_duration(m_pdIn, _nPollWaitMsec);

          if(pfring_enable_ring(m_pdIn))
          {
              m_pLog->error("pfring enable ring failure");
              break;
          }
      }
      //perhaps use pfring reflection in the future.
      if(m_ippsDoc.HasMember("interfaces_out"))
      {
          MSTS    _intfSts;
          string  _strDirection       = RXTXAL_MPFRING_OUT_DIRECTION;
          int     _nBindId            = RXTXAL_MPFRING_OUT_BINDID;
          int     _nWatermark         = RXTXAL_MPFRING_OUT_WATERMARK;
          int     _nPollWaitMsec      = RXTXAL_MPFRING_OUT_POLLWAIT_MSEC;
          int     _nRingClusterId     = RXTXAL_MPFRING_OUT_RING_CLUSTERID;
          bool    _bHwTimeStamp       = RXTXAL_MPFRING_OUT_HW_TIMESTAMP;
          bool    _bStripTimeStamp    = RXTXAL_MPFRING_OUT_STRIP_TIMESTAMP;
          int     _nSnapLength        = RXTXAL_MPFRING_OUT_SNAPLENGTH;
          unsigned long _flags        = 0;

          const Value& _intfField = m_ippsDoc["interfaces_out"];
          if(_intfField.HasMember("direction"))
              _strDirection = _intfField["direction"].GetString();
          if(_intfField.HasMember("core_bind_id"))
              _nBindId = _intfField["core_bind_id"].GetInt();
          if(_intfField.HasMember("watermark"))
              _nWatermark = _intfField["watermark"].GetInt();
          if(_intfField.HasMember("poll_wait_msec"))
              _nPollWaitMsec = _intfField["poll_wait_msec"].GetInt();
          if(_intfField.HasMember("ring_cluster_id"))
              _nRingClusterId = _intfField["ring_cluster_id"].GetInt();
          string _strIntfName = "";
          auto&  _lstNames = _intfField["name"];
          for (SizeType _i = 0; _i < _lstNames.Size(); _i++)
          {
              _strIntfName += _lstNames[_i].GetString();
              if (_i < _lstNames.Size()-1)
                  _strIntfName += ",";
          }
          m_pLog->debug("interfaces_out/snaplen: {}",_nSnapLength);
          m_pLog->debug("interfaces_out/direction: {}",_strDirection);
          m_pLog->debug("interfaces_out/core_bind_id: {}",_nBindId);
          m_pLog->debug("interfaces_out/watermark: {}",_nWatermark);
          m_pLog->debug("interfaces_out/poll_wait_msec: {}",_nPollWaitMsec);
          m_pLog->debug("interfaces_out/ring_cluster_id: {}",_nRingClusterId);
          m_pLog->debug("interfaces_out/hw_timestamp: {}",_bHwTimeStamp);
          m_pLog->debug("interfaces_out/strip_timestamp: {}",_bStripTimeStamp);
          m_pLog->debug("interfaces_out/name: {}",_strIntfName);

          if(_nThreads > 1)     _flags |= PF_RING_REENTRANT;
          if(_bHwTimeStamp)     _flags |= PF_RING_HW_TIMESTAMP;
          if(_bStripTimeStamp)  _flags |= PF_RING_STRIP_HW_TIMESTAMP;
          _flags |= PF_RING_LONG_HEADER | PF_RING_PROMISC;
          m_pdOut = pfring_open(_strIntfName.c_str(), _nSnapLength, _flags);
          if(!m_pdOut)
          {
              m_pLog->error("failure to perform pfring_open on intf {}",_strIntfName);
              break;
          }
          u_int32_t _version;

          pfring_set_application_name(m_pdOut,(char*)"ipps");
          pfring_version(m_pdOut, &_version);

          m_pLog->info("Using PF_RING v.{}.{}.{}",
             (_version & 0xFFFF0000) >> 16,
             (_version & 0x0000FF00) >> 8,
             _version & 0x000000FF);

          /*if(_nWatermark > 0)
          {
              _intfSts = pfring_set_tx_watermark(m_pdOut, _nWatermark);
              if(MDSUCCESS != _intfSts)
              {
                  m_pLog->error("pfring_set_tx_watermark() failed [rc={}]", _intfSts);
                  break;
              }
          }*/
          if(_nRingClusterId > 0)
          {
              _intfSts = pfring_set_cluster(m_pdOut, _nRingClusterId, cluster_hash_type);
              if(MDSUCCESS != _intfSts)
              {
                  m_pLog->error("pfring_set_cluster returned {}", _intfSts);
              }
          }
          packet_direction direction;
          if(boost::iequals(_strDirection, "rx"))
              direction = rx_only_direction;
          else if(boost::iequals(_strDirection, "tx"))
              direction = tx_only_direction;
          else if(boost::iequals(_strDirection, "rx+tx"))
              direction = rx_and_tx_direction;
          else
          {
              m_pLog->error("pfring_set_direction, options are rx,tx and rx+tx");
              break;
          }
          pfring_set_direction(m_pdOut, direction);
          _intfSts = pfring_set_socket_mode(m_pdOut, send_only_mode);
          if(MDSUCCESS != _intfSts)
          {
              m_pLog->error("pfring_set_socket_mode returned [_intfSts={}]", _intfSts);
              break;
          }

          if(MDSUCCESS != pfring_enable_ring(m_pdOut))
          {
              m_pLog->error("Unable to enable egress ring");
              break;
          }
      }
      _sts = MDSUCCESS;
  }while(FALSE);

  /* perform cleanup in case of error */
  if(MDSUCCESS != _sts)
  {
      if(m_pdIn)
          pfring_close(m_pdIn);
      if(m_pdOut)
          pfring_close(m_pdOut);
  }

  return _sts;
}

rxtxal_pkthdr* pfringDPI::getPktHeader()
{
    m_pLog->info("pfringDPI::getPktHeader()");
    m_pktHdr.caplen   = m_pfringPktHdr.caplen;
    m_pktHdr.eth_type = m_pfringPktHdr.extended_hdr.parsed_pkt.eth_type;
    return (&m_pktHdr);
}

int pfringDPI::write_rx(void* pbuffer, u_int size)
{
  int       _pktSent;

  m_pLog->info("pfringDPI::write_rx()");
  m_pLog->info("pfring caplen {}",size);
  _pktSent = pfring_send(m_pdIn, (char*)pbuffer, size,1);
  m_pLog->info("pfring send {}",_pktSent);
  return _pktSent;
}

int pfringDPI::write_tx(void* pbuffer, u_int size)
{
  int       _pktSent;

  m_pLog->info("pfringDPI::write_tx()");
  m_pLog->info("pfring caplen {}",size);
  _pktSent = pfring_send(m_pdOut, (char*)pbuffer, size,1);
  m_pLog->info("pfring send {}",_pktSent);
  return _pktSent;
}

int pfringDPI::read_rx(void* pbuffer, u_int bufSize)
{
  int       _pktRead;

  m_pLog->info("pfringDPI::read_rx()");
  _pktRead = pfring_recv(m_pdIn, (u_char**)&pbuffer,
          bufSize,
          &m_pfringPktHdr, m_waitForPacket);
  return _pktRead;
}

int pfringDPI::read_tx(void* pbuffer, u_int bufSize)
{
  int       _pktRead;

  m_pLog->info("pfringDPI::read_tx()");
  _pktRead = pfring_recv(m_pdOut, (u_char**)&pbuffer,
          bufSize,
          &m_pfringPktHdr, m_waitForPacket);
  return _pktRead;
}

MSTS pfringDPI::flush_rx()
{
  MSTS  _sts;

  m_pLog->info("pfringDPI::flush_rx()");
  _sts = pfring_flush_tx_packets(m_pdIn);
  return _sts;
}

MSTS pfringDPI::flush_tx()
{
  MSTS  _sts;

  m_pLog->info("pfringDPI::flush_tx()");
  _sts = pfring_flush_tx_packets(m_pdOut);
  return _sts;
}

MSTS pfringDPI::close_rx()
{
  m_pLog->info("pfringDPI::close_rx()");
  pfring_close(m_pdIn);
  return MDSUCCESS;
}

MSTS pfringDPI::close_tx()
{
  m_pLog->info("pfringDPI::close_tx()");
  pfring_close(m_pdOut);
  return MDSUCCESS;
}

u_char pfringDPI::isWaitForPacket(){
    m_pLog->info("pfringDPI::isWaitForPacket()");
    return (m_waitForPacket);
}

//TODO: temporary helper, delete this code
void pfringDPI::printPacket(int32_t tzone, void* pBuffer)
{
  int _s;
  u_int _usec, _nsec = 0;
  bool _extendedPktHdr = true;
  char _dumpStr[512] = { 0 };
  u_char* _pBuffer = (u_char*)pBuffer;

  m_pLog->info("pfringDPI::printPacket()");

 if(/*(!is_sysdig) &&*/ (m_pfringPktHdr.ts.tv_sec == 0 || m_pfringPktHdr.extended_hdr.parsed_pkt.offset.l3_offset == 0))
 {
    memset((void*)&m_pfringPktHdr.extended_hdr.parsed_pkt, 0, sizeof(struct pkt_parsing_info));
    pfring_parse_pkt((u_char*)_pBuffer, (struct pfring_pkthdr*)&m_pfringPktHdr, 5, 0, 1);
  }

 _s = (m_pfringPktHdr.ts.tv_sec + tzone) % 86400;

  if(m_pfringPktHdr.extended_hdr.timestamp_ns)
  {
    _s = ((m_pfringPktHdr.extended_hdr.timestamp_ns / 1000000000) + tzone) % 86400;
    _usec = (m_pfringPktHdr.extended_hdr.timestamp_ns / 1000) % 1000000;
    _nsec = m_pfringPktHdr.extended_hdr.timestamp_ns % 1000;
  }
  else
  {
    _usec = m_pfringPktHdr.ts.tv_usec;
  }

  snprintf(_dumpStr, sizeof(_dumpStr), "%02d:%02d:%02d.%06u%03u ",
          _s / 3600, (_s % 3600) / 60, _s % 60,
           _usec, _nsec);

  if(_extendedPktHdr)
  {
    char _bigbuf[4096];
    u_int _len;

    snprintf(&_dumpStr[strlen(_dumpStr)], sizeof(_dumpStr)-strlen(_dumpStr), "%s[if_index=%d]",
      m_pfringPktHdr.extended_hdr.rx_direction ? "[RX]" : "[TX]",
      m_pfringPktHdr.extended_hdr.if_index);

    pfring_print_parsed_pkt(_bigbuf, sizeof(_bigbuf), _pBuffer, &m_pfringPktHdr);
    _len = strlen(_bigbuf);

    if(_len > 0) _bigbuf[_len-1] = '\0';

    snprintf(&_dumpStr[strlen(_dumpStr)], sizeof(_dumpStr)-strlen(_dumpStr), "%s", _bigbuf);
  }
  else
  {
    char _buf1[32], _buf2[32];
    struct ether_header *ehdr = (struct ether_header *) pBuffer;

    snprintf(&_dumpStr[strlen(_dumpStr)], sizeof(_dumpStr)-strlen(_dumpStr),
             "[%s -> %s][eth_type=0x%04X][caplen=%d][len=%d]",
             etheraddrString(ehdr->ether_shost, _buf1),
             etheraddrString(ehdr->ether_dhost, _buf2),
         ntohs(ehdr->ether_type),
         m_pfringPktHdr.caplen, m_pfringPktHdr.len);
  }
  m_pLog->info("{}",_dumpStr);
}

//TODO: temporary helper, replace this  code
char* pfringDPI::etheraddrString(const u_char *ep, char *buf)
{
  char *hex = (char*)"0123456789ABCDEF";
  u_int i, j;
  char *cp;

  cp = buf;
  if((j = *ep >> 4) != 0)
    *cp++ = hex[j];
  else
    *cp++ = '0';

  *cp++ = hex[*ep++ & 0xf];

  for(i = 5; (int)--i >= 0;) {
    *cp++ = ':';
    if((j = *ep >> 4) != 0)
      *cp++ = hex[j];
    else
      *cp++ = '0';

    *cp++ = hex[*ep++ & 0xf];
  }

  *cp = '\0';
  return (buf);
}

