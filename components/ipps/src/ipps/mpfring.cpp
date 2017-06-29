#include "ipps.h"

MSTS mpfring::init()

{
    MSTS    _sts                = MDERROR;
    int     _nThreads           = IPPS_MPFRING_THREADS;
    u_char  _mac_address[6]     = { 0 };
    char    _buf[32];

    do
    {
        _nThreads = ippsDoc["threads"].GetInt();
        IPPSLOG->debug("threads: {}",_nThreads);
        // default flow to 5 tuple for rx for now
        cluster_type cluster_hash_type = cluster_per_flow_5_tuple;

        if(ippsDoc.HasMember("interfaces_in"))
        {
            MSTS    _intfSts;
            string  _strDirection       = IPPS_MPFRING_IN_DIRECTION;
            int     _nBindId            = IPPS_MPFRING_IN_BINDID;
            int     _nWatermark         = IPPS_MPFRING_IN_WATERMARK;
            int     _nPollWaitMsec      = IPPS_MPFRING_IN_POLLWAIT_MSEC;
            int     _nRingClusterId     = IPPS_MPFRING_IN_RING_CLUSTERID;
            bool    _bHwTimeStamp       = IPPS_MPFRING_IN_HW_TIMESTAMP;
            bool    _bStripTimeStamp    = IPPS_MPFRING_IN_STRIP_TIMESTAMP;
            int     _nSnapLength        = IPPS_MPFRING_IN_SNAPLENGTH;
            u_int32_t _flags            = 0;

            const Value& _intfField = ippsDoc["interfaces_in"];
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

            IPPSLOG->debug("interfaces_in/snaplen: {}",_nSnapLength);
            IPPSLOG->debug("interfaces_in/direction: {}",_strDirection);
            IPPSLOG->debug("interfaces_in/core_bind_id: {}",_nBindId);
            IPPSLOG->debug("interfaces_in/watermark: {}",_nWatermark);
            IPPSLOG->debug("interfaces_in/poll_wait_msec: {}",_nPollWaitMsec);
            IPPSLOG->debug("interfaces_in/ring_cluster_id: {}",_nRingClusterId);
            IPPSLOG->debug("interfaces_in/hw_timestamp: {}",_bHwTimeStamp);
            IPPSLOG->debug("interfaces_in/strip_timestamp: {}",_bStripTimeStamp);
            IPPSLOG->debug("interfaces_in/name: {}",_strIntfName);
            IPPSLOG->debug("interfaces_in/pcap_filters: {}",_strPcapFilter);

            if(_nThreads > 1)     _flags |= PF_RING_REENTRANT;
            if(_bHwTimeStamp)     _flags |= PF_RING_HW_TIMESTAMP;
            if(_bStripTimeStamp)  _flags |= PF_RING_STRIP_HW_TIMESTAMP;
            _flags |= PF_RING_LONG_HEADER | PF_RING_PROMISC;

            /* common from here */
            pdIn = pfring_open(_strIntfName.c_str(), _nSnapLength, _flags);
            if(!pdIn)
            {
                IPPSLOG->error("failure to perform pfring_open on intf {}",_strIntfName);
                break;
            }
            u_int32_t _version;

            pfring_set_application_name(pdIn,(char*)"ipps");
            pfring_version(pdIn, &_version);

            IPPSLOG->info("Using PF_RING v.{}.{}.{}",
               (_version & 0xFFFF0000) >> 16,
               (_version & 0x0000FF00) >> 8,
               _version & 0x000000FF);

            if(pfring_get_bound_device_address(pdIn, _mac_address) != 0)
                IPPSLOG->warn("Unable to read the device address");
            else
            {
                int _ifindex = -1;

                pfring_get_bound_device_ifindex(pdIn, &_ifindex);

                IPPSLOG->info("Capturing from {} [mac: {}][if_index: {}][speed: {}Mb/s]",
                        (char*)_strIntfName.c_str(), etheraddrString(_mac_address, _buf),
                   _ifindex,
                       pfring_get_interface_speed(pdIn));
            }

            IPPSLOG->info("# Device RX channels: {}", pfring_get_num_rx_channels(pdIn));
            IPPSLOG->info("# Polling threads:    {}", _nThreads);

            /* setup bpf filtering, change this to pfring one in the future */
            _intfSts = pfring_set_bpf_filter(pdIn, (char*)(_strPcapFilter.c_str()));
            if(_intfSts != MDSUCCESS)
            {
                IPPSLOG->error("pfring_set_bpf_filter({}) returned {}", _strPcapFilter.c_str(), _intfSts);
                break;
            }
            else
                IPPSLOG->info("Successfully set BPF filter '{}'", _strPcapFilter.c_str());

            packet_direction direction;
            if(boost::iequals(_strDirection, "rx"))
                direction = rx_only_direction;
            else if(boost::iequals(_strDirection, "tx"))
                direction = tx_only_direction;
            else if(boost::iequals(_strDirection, "rx+tx"))
                direction = rx_and_tx_direction;
            else
            {
                IPPSLOG->error("pfring_set_direction, options are rx,tx and rx+tx");
                break;
            }
            pfring_set_direction(pdIn, direction);
            _intfSts = pfring_set_socket_mode(pdIn, recv_only_mode);
            if(MDSUCCESS != _intfSts)
            {
                IPPSLOG->error("pfring_set_socket_mode returned [_intfSts={}]", _intfSts);
                break;
            }

            if(_nWatermark > 0)
            {
              _intfSts = pfring_set_poll_watermark(pdIn, _nWatermark);
              if(MDSUCCESS != _intfSts)
              {
                  IPPSLOG->error("pfring_set_poll_watermark returned [rc={}][watermark={}]", _intfSts, _nWatermark);
                  break;
              }
            }
            else
            {
                IPPSLOG->error("pfring_set_poll_watermark must be > 0");
                break;
            }
            if(_nRingClusterId > 0)
            {
                _intfSts = pfring_set_cluster(pdIn, _nRingClusterId, cluster_hash_type);
                if(MDSUCCESS != _intfSts)
                {
                    IPPSLOG->error("pfring_set_cluster returned {}", _intfSts);
                }
            }
            if(_nPollWaitMsec > 0)
                pfring_set_poll_duration(pdIn, _nPollWaitMsec);

            if(pfring_enable_ring(pdIn))
            {
                IPPSLOG->error("pfring enable ring failure");
                break;
            }
        }
        //perhaps use pfring reflection in the future.
        if(ippsDoc.HasMember("interfaces_out"))
        {
            MSTS    _intfSts;
            string  _strDirection       = IPPS_MPFRING_OUT_DIRECTION;
            int     _nBindId            = IPPS_MPFRING_OUT_BINDID;
            int     _nWatermark         = IPPS_MPFRING_OUT_WATERMARK;
            int     _nPollWaitMsec      = IPPS_MPFRING_OUT_POLLWAIT_MSEC;
            int     _nRingClusterId     = IPPS_MPFRING_OUT_RING_CLUSTERID;
            bool    _bHwTimeStamp       = IPPS_MPFRING_OUT_HW_TIMESTAMP;
            bool    _bStripTimeStamp    = IPPS_MPFRING_OUT_STRIP_TIMESTAMP;
            int     _nSnapLength        = IPPS_MPFRING_OUT_SNAPLENGTH;
            unsigned long _flags        = 0;

            const Value& _intfField = ippsDoc["interfaces_out"];
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
            IPPSLOG->debug("interfaces_out/snaplen: {}",_nSnapLength);
            IPPSLOG->debug("interfaces_out/direction: {}",_strDirection);
            IPPSLOG->debug("interfaces_out/core_bind_id: {}",_nBindId);
            IPPSLOG->debug("interfaces_out/watermark: {}",_nWatermark);
            IPPSLOG->debug("interfaces_out/poll_wait_msec: {}",_nPollWaitMsec);
            IPPSLOG->debug("interfaces_out/ring_cluster_id: {}",_nRingClusterId);
            IPPSLOG->debug("interfaces_out/hw_timestamp: {}",_bHwTimeStamp);
            IPPSLOG->debug("interfaces_out/strip_timestamp: {}",_bStripTimeStamp);
            IPPSLOG->debug("interfaces_out/name: {}",_strIntfName);

            if(_nThreads > 1)     _flags |= PF_RING_REENTRANT;
            if(_bHwTimeStamp)     _flags |= PF_RING_HW_TIMESTAMP;
            if(_bStripTimeStamp)  _flags |= PF_RING_STRIP_HW_TIMESTAMP;
            _flags |= PF_RING_LONG_HEADER | PF_RING_PROMISC;
            pdOut = pfring_open(_strIntfName.c_str(), _nSnapLength, _flags);
            if(!pdOut)
            {
                IPPSLOG->error("failure to perform pfring_open on intf {}",_strIntfName);
                break;
            }
            u_int32_t _version;

            pfring_set_application_name(pdOut,(char*)"ipps");
            pfring_version(pdOut, &_version);

            IPPSLOG->info("Using PF_RING v.{}.{}.{}",
               (_version & 0xFFFF0000) >> 16,
               (_version & 0x0000FF00) >> 8,
               _version & 0x000000FF);

            /*if(_nWatermark > 0)
            {
                _intfSts = pfring_set_tx_watermark(pdOut, _nWatermark);
                if(MDSUCCESS != _intfSts)
                {
                    IPPSLOG->error("pfring_set_tx_watermark() failed [rc={}]", _intfSts);
                    break;
                }
            }*/
            if(_nRingClusterId > 0)
            {
                _intfSts = pfring_set_cluster(pdIn, _nRingClusterId, cluster_hash_type);
                if(MDSUCCESS != _intfSts)
                {
                    IPPSLOG->error("pfring_set_cluster returned {}", _intfSts);
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
                IPPSLOG->error("pfring_set_direction, options are rx,tx and rx+tx");
                break;
            }
            pfring_set_direction(pdIn, direction);
            _intfSts = pfring_set_socket_mode(pdOut, send_only_mode);
            if(MDSUCCESS != _intfSts)
            {
                IPPSLOG->error("pfring_set_socket_mode returned [_intfSts={}]", _intfSts);
                break;
            }

            if(MDSUCCESS != pfring_enable_ring(pdOut))
            {
                IPPSLOG->error("Unable to enable egress ring");
                break;
            }
        }
        _sts = MDSUCCESS;
    }while(FALSE);

    /* perform cleanup in case of error */
    if(MDSUCCESS != _sts)
    {
        if(pdIn)
            pfring_close(pdIn);
        if(pdOut)
            pfring_close(pdOut);
    }

    return _sts;
}

