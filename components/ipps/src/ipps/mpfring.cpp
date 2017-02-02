#include "ipps.h"

MSTS mpfring::init()

{
    MSTS    _sts                = MDERROR;
    int     _nThreads           = IPPS_MPFRING_THREADS;

    do
    {
        string  _strDirection       = IPPS_MPFRING_IN_DIRECTION;
        int     _nBindId            = IPPS_MPFRING_IN_BINDID;
        int     _nWatermark         = IPPS_MPFRING_IN_WATERMARK;
        int     _nPollWaitMsec      = IPPS_MPFRING_IN_POLLWAIT_MSEC;
        int     _nRingClusterId     = IPPS_MPFRING_IN_RING_CLUSTERID;
        bool    _bHwTimeStamp       = IPPS_MPFRING_IN_HW_TIMESTAMP;
        bool    _bStripTimeStamp    = IPPS_MPFRING_IN_STRIP_TIMESTAMP;
        int     _nSnapLength        = IPPS_MPFRING_IN_SNAPLENGTH;
        unsigned long _flags        = 0;

        _nThreads = ippsDoc["threads"].GetInt();
        pMIppsLog->debug("threads: {}",_nThreads);
        if(ippsDoc.HasMember("interfaces_in"))
        {
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
            pMIppsLog->debug("interfaces_in/snaplen: {}",_nSnapLength);
            pMIppsLog->debug("interfaces_in/direction: {}",_strDirection);
            pMIppsLog->debug("interfaces_in/core_bind_id: {}",_nBindId);
            pMIppsLog->debug("interfaces_in/watermark: {}",_nWatermark);
            pMIppsLog->debug("interfaces_in/poll_wait_msec: {}",_nPollWaitMsec);
            pMIppsLog->debug("interfaces_in/ring_cluster_id: {}",_nRingClusterId);
            pMIppsLog->debug("interfaces_in/hw_timestamp: {}",_bHwTimeStamp);
            pMIppsLog->debug("interfaces_in/strip_timestamp: {}",_bStripTimeStamp);
            pMIppsLog->debug("interfaces_in/name: {}",_strIntfName);

            if(_nThreads > 1)     _flags |= PF_RING_REENTRANT;
            if(_bHwTimeStamp)     _flags |= PF_RING_HW_TIMESTAMP;
            if(_bStripTimeStamp)  _flags |= PF_RING_STRIP_HW_TIMESTAMP;
            _flags |= PF_RING_LONG_HEADER | PF_RING_PROMISC;
            /*pdIn = pfring_open(_strIntfName.c_str(), _nSnapLength, _flags);
            if(!pdIn)
            {
                pMIppsLog->error("failure to perform pfring_open on intf {}",_strIntfName);
                break;
            }
            pfring_close(pdIn);
            */
        }
        //perhaps use pfring reflection in the future.
        if(ippsDoc.HasMember("interfaces_out"))
        {
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
            pMIppsLog->debug("interfaces_out/snaplen: {}",_nSnapLength);
            pMIppsLog->debug("interfaces_out/direction: {}",_strDirection);
            pMIppsLog->debug("interfaces_out/core_bind_id: {}",_nBindId);
            pMIppsLog->debug("interfaces_out/watermark: {}",_nWatermark);
            pMIppsLog->debug("interfaces_out/poll_wait_msec: {}",_nPollWaitMsec);
            pMIppsLog->debug("interfaces_out/ring_cluster_id: {}",_nRingClusterId);
            pMIppsLog->debug("interfaces_out/hw_timestamp: {}",_bHwTimeStamp);
            pMIppsLog->debug("interfaces_out/strip_timestamp: {}",_bStripTimeStamp);
            pMIppsLog->debug("interfaces_out/name: {}",_strIntfName);


            if(_nThreads > 1)     _flags |= PF_RING_REENTRANT;
            if(_bHwTimeStamp)     _flags |= PF_RING_HW_TIMESTAMP;
            if(_bStripTimeStamp)  _flags |= PF_RING_STRIP_HW_TIMESTAMP;
            _flags |= PF_RING_LONG_HEADER | PF_RING_PROMISC;
            /*pdOut = pfring_open(_strIntfName.c_str(), _nSnapLength, _flags);
            if(!pdOut)
            {
                pMIppsLog->error("failure to perform pfring_open on intf {}",_strIntfName);
                break;
            }
            pfring_close(pdOut);*/
        }

        _sts = MDSUCCESS;
    }while(FALSE);

    return _sts;
}

