#ifndef _MPFRING_H_
#define _MPFRING_H_

#include "pfring.h"

#define IPPS_MPFRING_THREADS               1

//Default settings ingress
#define IPPS_MPFRING_IN_BINDID             1
#define IPPS_MPFRING_IN_WATERMARK          10
#define IPPS_MPFRING_IN_POLLWAIT_MSEC      100
#define IPPS_MPFRING_IN_RING_CLUSTERID     55
#define IPPS_MPFRING_IN_HW_TIMESTAMP       false
#define IPPS_MPFRING_IN_STRIP_TIMESTAMP    true
#define IPPS_MPFRING_IN_SNAPLENGTH         2048
#define IPPS_MPFRING_IN_DIRECTION          "rx"

//Default settings ingress
#define IPPS_MPFRING_OUT_BINDID             2
#define IPPS_MPFRING_OUT_WATERMARK          100
#define IPPS_MPFRING_OUT_POLLWAIT_MSEC      10
#define IPPS_MPFRING_OUT_RING_CLUSTERID     65
#define IPPS_MPFRING_OUT_HW_TIMESTAMP       false
#define IPPS_MPFRING_OUT_STRIP_TIMESTAMP    true
#define IPPS_MPFRING_OUT_SNAPLENGTH         2048
#define IPPS_MPFRING_OUT_DIRECTION          "tx"

class mpfring {

  public:
    mpfring(bool bver, std::shared_ptr<spdlog::logger> pLog, Document *pInDoc) :
        bVerbose(bver),pdIn(NULL), pdOut(NULL)
    {
         pMIppsLog = pLog;
        //Do deep copy here.
        ippsDoc.CopyFrom(*pInDoc, ippsDoc.GetAllocator());

    }
    MSTS    init();
    pfring* getPfringIngress(){return pdIn;}
    pfring* getPfringEgress(){return pdOut;}
   // log()

  private:
    bool                                bVerbose;
    std::shared_ptr<spdlog::logger>     pMIppsLog;
    Document                            ippsDoc;
    pfring*                             pdIn;
    pfring*                             pdOut;
};

#endif /* _MPFRING_H_ */
