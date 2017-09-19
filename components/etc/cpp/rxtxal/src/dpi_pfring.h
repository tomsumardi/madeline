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
      pfringDPI(bool bver, std::shared_ptr<spdlog::logger> pCLog, Document *pInDoc) :
          bVerbose(bver),pdIn(NULL), pdOut(NULL)
      {
           pLog = pCLog;
          //Do deep copy here.
          ippsDoc.CopyFrom(*pInDoc, ippsDoc.GetAllocator());
          /* Pfring ingress descriptor */
          pdIn = NULL;
          /* Pfring ingress descriptor */
          pdOut = NULL;

      }
      ~pfringDPI() {
        cout << "pfringDPI" << "::dtor" << endl;
      }
      void exec() {
        cout << "test::exec()" << endl;
      }
      // Default operations
      MSTS open();
      MSTS write();
      MSTS read();
      MSTS close();
      MSTS init();
      pfring* getPfringIngress(){return pdIn;}
      pfring* getPfringEgress(){return pdOut;}
  private:
      bool                                bVerbose;
      std::shared_ptr<spdlog::logger>     pLog;
      Document                            ippsDoc;
      pfring*                             pdIn;
      pfring*                             pdOut;
};

#endif /* _DPI_PFRING_H_ */
