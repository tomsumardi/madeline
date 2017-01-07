#ifndef _MPARSE_H_
#define _MPARSE_H_

#include "../mutil/src/madeline.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include <string>

using namespace std;
namespace ipps_proptree = boost::property_tree;

class ippsJson{

  public:
    ippsJson(){};
    void setJsonLoc(string strloc){strJsonTxtLoc = strloc;}
    MSTS parse();
    // log()

  private:
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

};


#endif /* _MPARSE_H_ */
