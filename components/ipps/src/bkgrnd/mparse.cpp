#include "mparse.h"

MSTS ippsJson::read()
{
    MSTS    _sts = MDERROR;

    try
    {
        ifstream    _f;
        string      _strLine;
        _f.open (strJsonTxtLoc);
        if (_f.is_open())
        {
            while(!_f.eof())
            {
                getline(_f,_strLine);
                strJsonTxt +=_strLine;
            }
            _f.close();
            _sts = MDSUCCESS;
        }
    }
    catch (exception const& e)
    {
      cerr << e.what() << endl;
    }
    return _sts;
}

MSTS ippsJson::parse()
{
    MSTS    _sts = MDERROR;
    try
    {
        cout << boost::format("unmarshalling: %s\n") %strJsonTxtLoc;
        cout << boost::format("%s\n") %strJsonTxt;

        configDoc.Parse(strJsonTxt.c_str());
        BOOST_ASSERT(configDoc.IsObject());

        BOOST_ASSERT(configDoc.HasMember("version"));
        BOOST_ASSERT(configDoc["version"].IsString());
        cout << boost::format("version = %s\n") %configDoc["version"].GetString();

        const Value& intfList = configDoc["interfaces"];
        BOOST_ASSERT(configDoc.HasMember("interfaces"));
        BOOST_ASSERT(configDoc["interfaces"].IsArray());

        // rapidjson uses SizeType instead of size_t.
        // name and direction are required values
        for (SizeType i = 0; i < intfList.Size(); i++)
        {
            const Value& intfElm = intfList[i];

            if(intfElm.HasMember("name"))
            {
                BOOST_ASSERT(intfElm["name"].IsString());
                cout << boost::format("name = %s\n") %intfElm["name"].GetString();
            }
            if(intfElm.HasMember("direction"))
            {
                BOOST_ASSERT(intfElm["direction"].IsString());
                cout << boost::format("direction = %s\n") %intfElm["direction"].GetString();
            }
        }
        _sts = MDSUCCESS;
    }
    catch (exception const& e)
    {
      cerr << e.what() << endl;
    }
    return _sts;
}

/*
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace ipps_proptree = boost::property_tree;

int ippsJson::parse()
{
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

    try
    {


        //BOOST_ASSERT(strJsonTxtLoc)
        //Might change below to c++ rapidJson DOM library

        cout << "unmarshalling:" << strJsonTxtLoc << "\n";
        ipps_proptree::read_json(strJsonTxtLoc, rootJson);

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
