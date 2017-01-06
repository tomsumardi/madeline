#include "mparse.h"

int ippsJson::parse()
{

    try
    {

        //BOOST_ASSERT(strJsonTxtLoc)
        //Might change below to c++ rapidJson DOM library

        cout << "unmarshalling:" << strJsonTxtLoc << "\n";
        boost::property_tree::read_json(strJsonTxtLoc, rootJson);

        // A vector to allow storing our interfaces
        vector< pair<string, string> > interfaces;

        // Iterator over all interfaces
        for (boost::property_tree::ptree::value_type &lstintf : rootJson.get_child("interfaces"))
        {
            ippsInterf _intf;
            for (boost::property_tree::ptree::value_type &interface : lstintf.second)
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
                    for (boost::property_tree::ptree::value_type &pcap_filters : interface.second)
                    {
                        //std::string value = pcap_filters.second.data();
                        _intf.pcap_filters.push_back(pcap_filters.second.data());
                    }
                }
                else if(0 == key.compare("l3_blacklists"))
                {
                    for (boost::property_tree::ptree::value_type &l3_blacklists : interface.second)
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

