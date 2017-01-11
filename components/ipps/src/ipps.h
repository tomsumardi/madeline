#ifndef _IPPS_H_
#define _IPPS_H_

#include "../mutil/src/madeline.h"
#include "../mutil/src/mutil.h"
#include <iostream>
#include <glib.h>

class ipps : public madeline{
  public:
    ipps(){setVerbose(false);setStandalone(false);};
    Document* getJsonDoc() {return(&jsonDoc);}
    bool getStandalone() {return(bMstandalone);}
    void setStandalone(bool bStandalone) {bMstandalone = bStandalone;}
    bool getVerbose() {return(bMverbose);}
    void setVerbose(bool bVerbose) {bMverbose = bVerbose;}
  private:
    Document    jsonDoc;
    //ippsPfring  pring;
    //ippsThread  thread;
};


#endif /* _IPPS_H_ */
