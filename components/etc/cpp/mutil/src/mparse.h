#ifndef _MPARSE_H_
#define _MPARSE_H_

#include "madeline.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

using namespace rapidjson;

class mutilMparse{

  public:
    mutilMparse()
    {
        strJsonTxt = "";
        strJsonLoc = "";
        pJsonDoc = NULL;
    };
    mutilMparse(string strLoc,
            Document* pDoc,
            std::shared_ptr<spdlog::logger> pLog)
    {
        strJsonTxt = "";
        strJsonLoc = strLoc;
        pJsonDoc = pDoc;
        pMIppsLog = pLog;
    };
    MSTS read();
    MSTS parse();
    MSTS processJson();
    // log()

  private:
    std::shared_ptr<spdlog::logger>     pMIppsLog;
    Document                            *pJsonDoc;
    string                              strJsonTxt;
    string                              strJsonLoc;

};


#endif /* _MPARSE_H_ */
