#ifndef _MPARSE_H_
#define _MPARSE_H_

#include "madeline.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

using namespace rapidjson;

class mutilMparse{

  public:
    mutilMparse()
    {
        strJsonLoc = "";
        pJsonDoc = NULL;
        pSchemaDoc = NULL;
    };
    mutilMparse(string strJLoc,
            Document* pJDoc,
            string strSLoc,
            Document* pSDoc,
            std::shared_ptr<spdlog::logger> pLog)
    {
        strJsonLoc = strJLoc;
        pJsonDoc = pJDoc;
        strSchemaLoc = strSLoc;
        pSchemaDoc = pSDoc;
        pMIppsLog = pLog;
    };
    string read(string strFpathName);
    MSTS parse(string strJTxt, Document* pJDoc);
    MSTS processJson();
    MSTS validate();
    // log()

  private:
    // dangerous passing pointer here, will need to do
    //deep copy
    std::shared_ptr<spdlog::logger>     pMIppsLog;
    Document                            *pJsonDoc;
    string                              strJsonLoc;
    Document                            *pSchemaDoc;
    string                              strSchemaLoc;

};


#endif /* _MPARSE_H_ */
