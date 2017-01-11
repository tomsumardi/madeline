#ifndef _MPARSE_H_
#define _MPARSE_H_

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "madeline.h"

class mutilMparse{

  public:
    mutilMparse()
    {
        strJsonTxt = "";
        strJsonLoc = "";
        pJsonDoc = NULL;
    };
    mutilMparse(string strLoc,Document* pDoc)
    {
        strJsonTxt = "";
        strJsonLoc = strLoc;
        pJsonDoc = pDoc;
    };
    MSTS read();
    MSTS parse();
    // log()

  private:
    Document                    *pJsonDoc;
    string                      strJsonTxt;
    string                      strJsonLoc;

};


#endif /* _MPARSE_H_ */
