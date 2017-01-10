#ifndef _MPARSE_H_
#define _MPARSE_H_

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "../mutil/src/madeline.h"
#include <boost/foreach.hpp>

class ippsJson{

  public:
    ippsJson()
    {
        strJsonTxt = "";
        strJsonTxtLoc = "";
    };
    void setJsonLoc(string strloc){strJsonTxtLoc = strloc;}
    MSTS read();
    MSTS parse();
    // log()

  private:
    Document                    configDoc;
    string                      strJsonTxt;
    string                      strJsonTxtLoc;

};


#endif /* _MPARSE_H_ */
