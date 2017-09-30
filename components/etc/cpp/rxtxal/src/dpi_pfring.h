#ifndef _DPI_PFRING_H_
#define _DPI_PFRING_H_

#include "rxtxal.h"
#include "pfring.h"
#include <iostream>
#include <glib.h>
#include <boost/assert.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

class pfringDPI {
  public:
      pfringDPI() {}
      pfringDPI(bool bver, std::shared_ptr<spdlog::logger> pLog, Document *pDoc) :
          m_bVerbose(bver),m_waitForPacket(1), m_pLog(pLog), m_pBuffer(NULL), m_pdIn(NULL), m_pdOut(NULL)
      {
          //Do deep copy here.
          m_ippsDoc.CopyFrom(*pDoc, m_ippsDoc.GetAllocator());
          memset(&m_pfringPktHdr, 0, sizeof(m_pfringPktHdr));
          memset(&m_pktHdr, 0, sizeof(m_pktHdr));

      }
      ~pfringDPI() {
        cout << "pfringDPI" << "::dtor" << endl;
        if(m_pBuffer)
            free(m_pBuffer);
      }
      //pfring specific function pointer
      rxtxal_pkthdr* getPktHeader();
      // Default operations
      MSTS      open(u_int bufSize);
      int       write(u_int size);
      int       read(u_int bufSize);
      MSTS      flush();
      MSTS      close();
      u_char    isWaitForPacket();
      u_char*   getBufferPtr();
      void      printPacket(int32_t tzone);
      // internal functions
      char* etheraddrString(const u_char *ep, char *buf);
  private:
      bool                                m_bVerbose;
      unsigned char                       m_waitForPacket;
      std::shared_ptr<spdlog::logger>     m_pLog;
      u_char*                             m_pBuffer;
      rxtxal_pkthdr                       m_pktHdr;
      Document                            m_ippsDoc;
      // Pfring specific
      pfring*                             m_pdIn;
      pfring*                             m_pdOut;
      struct pfring_pkthdr                m_pfringPktHdr;
};

#endif /* _DPI_PFRING_H_ */
