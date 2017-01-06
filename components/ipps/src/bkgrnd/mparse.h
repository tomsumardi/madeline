#ifndef _MPARSE_H_
#define _MPARSE_H_

#include "../mutil/src/madeline.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include <string>

using namespace std;

class ippsJson{
    typedef struct interfaces {
        string name;
        string direction;
        vector< string > pcap_filters;
        vector< string > l3_blacklists;
    }ippsInterf;

    typedef struct config{
        string version;
        vector< ippsInterf > interfaces;
    }ippsConfig;

  boost::property_tree::ptree rootJson;
  ippsConfig                  ippsConf;
  string                      strJsonTxtLoc;

  public:
    ippsJson(string jsonTxt)
    {strJsonTxtLoc = jsonTxt;}
    int parse();
   // log()
};
/*

class ippsCmd {

  public:
    parseCmd()
    logging()
};
*/


#endif /* _MPARSE_H_ */
