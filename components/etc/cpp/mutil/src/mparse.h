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
#define MPARSELOG             m_pMParseLog
#define MPARSE_STACKTRACE(e)  DUMP_STACKTRACE_LOG(e, MPARSELOG)

class mutilMparse : public madeline{

  public:
    mutilMparse()
    {
        m_strJsonLoc = "";
        m_pJsonDoc = NULL;
        m_pSchemaDoc = NULL;
    };
    mutilMparse(string strJLoc,
            Document* pJDoc,
            string strSLoc,
            Document* pSDoc,
            std::shared_ptr<spdlog::logger> pLog)
    {
        m_strJsonLoc = strJLoc;
        m_pJsonDoc = pJDoc;
        m_strSchemaLoc = strSLoc;
        m_pSchemaDoc = pSDoc;
        m_pMParseLog = pLog;
    };
    string read(string strFpathName);
    MSTS parse(string strJTxt, Document* pJDoc);
    MSTS processJson();
    MSTS validate();
    // log()

  private:
    // dangerous passing pointer here, will need to do
    //deep copy
    std::shared_ptr<spdlog::logger>     m_pMParseLog;
    Document                            *m_pJsonDoc;
    string                              m_strJsonLoc;
    Document                            *m_pSchemaDoc;
    string                              m_strSchemaLoc;

};


#endif /* _MPARSE_H_ */
