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
  typedef struct mjson {
    int test;
  }ippsMjson;
  boost::property_tree::ptree rootJson;
  ippsMjson jsonObj;

  public:
    ippsJson(string jsonTxt)
    {
      readLocal(jsonTxt);
    }
    int readLocal(string jsonTxt);
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
