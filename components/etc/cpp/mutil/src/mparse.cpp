#include "mparse.h"

string mutilMparse::read(string strFpathName)
{
    string    _strTxt = "";

    try
    {
        ifstream    _f;
        string      _strLine;
        _f.open (strFpathName);
        if (_f.is_open())
        {
            while(!_f.eof())
            {
                getline(_f,_strLine);
                _strTxt +=_strLine;
            }
            _f.close();
        }
    }
    catch (exception const& e)
    {
      cerr << e.what() << endl;
    }
    return _strTxt;
}

MSTS mutilMparse::parse(string strJTxt, Document* pJDoc)
{
    MSTS    _sts = MDERROR;
    try
    {
        pMIppsLog->debug("unmarshalling: {}",strJsonLoc);
        pMIppsLog->debug("{}",strJTxt);

        pJDoc->Parse(strJTxt.c_str());
        BOOST_ASSERT(pJDoc->IsObject());
        _sts = MDSUCCESS;
    }
    catch (exception const& e)
    {
      cerr << e.what() << endl;
    }
    return _sts;
}

MSTS mutilMparse::processJson()
{
    MSTS      _sts = MDERROR;
    string    _strTxt = "";
    do
    {
         _strTxt = mutilMparse::read(strJsonLoc);
         if(boost::iequals(_strTxt,""))
         {
             pMIppsLog->error("failed to read json input file {}",strJsonLoc);
             break;
         }
         _sts = mutilMparse::parse(_strTxt,pJsonDoc);
         if(_sts != MDSUCCESS)
         {
             pMIppsLog->error("failed to parse json input file {}",strJsonLoc);
             break;
         }
         _strTxt = mutilMparse::read(strSchemaLoc);
         if(boost::iequals(_strTxt,""))
         {
             pMIppsLog->error("failed to read json input file {}",strSchemaLoc);
             break;
         }
         _sts = mutilMparse::parse(_strTxt,pSchemaDoc);
         if(_sts != MDSUCCESS)
         {
             pMIppsLog->error("failed to parse json input file {}",strSchemaLoc);
             break;
         }
         _sts = MDSUCCESS;
    }while(FALSE);

    return _sts;
}

MSTS mutilMparse::validate()
{
    MSTS    _sts = MDSUCCESS;
    SchemaDocument schema(*pSchemaDoc); // Compile a Document to SchemaDocument
    SchemaValidator validator(schema);
    if (!pJsonDoc->Accept(validator)) {
        // Input JSON is invalid according to the schema
        // Output diagnostic information
        StringBuffer sb;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        pMIppsLog->error("Invalid schema: {}", sb.GetString());
        pMIppsLog->error("Invalid keyword: {}", validator.GetInvalidSchemaKeyword());
        sb.Clear();
        validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
        pMIppsLog->error("Invalid document: {}", sb.GetString());
        _sts = MDERROR;
    }
    return _sts;
}

/*
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace ipps_proptree = boost::property_tree;

int mutilMparse::parse()
{
    namespace ipps_proptree = boost::property_tree;

    class mutilMparse{

      public:
        mutilMparse(){};
        void setstrJsonLoc(string strloc){strJsonLoc = strloc;}
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
        string                      strJsonLoc;

    };

    try
    {


        //BOOST_ASSERT(strJsonLoc)
        //Might change below to c++ rapidJson DOM library

        cout << "unmarshalling:" << strJsonLoc << "\n";
        ipps_proptree::read_json(strJsonLoc, rootJson);

        // Iterator over all interfaces
        for (ipps_proptree::ptree::value_type &lstintf : rootJson.get_child("interfaces"))
        {
            ippsInterf _intf;
            for (ipps_proptree::ptree::value_type &interface : lstintf.second)
            {
                // Get the label of the node
                std::string key = interface.first;
                if (0 == key.compare("name"))
                {
                    // Get the content of the node
                    _intf.name = interface.second.data();
                }
                else if(0 == key.compare("direction"))
                {
                    _intf.direction = interface.second.data();
                }
                else if(0 == key.compare("pcap_filters"))
                {
                    for (ipps_proptree::ptree::value_type &pcap_filters : interface.second)
                    {
                        //std::string value = pcap_filters.second.data();
                        _intf.pcap_filters.push_back(pcap_filters.second.data());
                    }
                }
                else if(0 == key.compare("l3_blacklists"))
                {
                    for (ipps_proptree::ptree::value_type &l3_blacklists : interface.second)
                    {
                        //std::string value = l3_blacklists.second.data();
                        _intf.l3_blacklists.push_back(l3_blacklists.second.data());
                    }
                }
            }
            ippsConf.interfaces.push_back(_intf);
        }
        return MDSUCCESS;
    }
    catch (exception const& e)
    {
      cerr << e.what() << endl;
    }
    return MDERROR;
}
*/
