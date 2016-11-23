#include "mparse.h"

int ippsJson::readLocal(string location)
{
    try
    {
        boost::property_tree::read_json(location, rootJson);

        // A vector to allow storing our interfaces
        vector< pair<string, string> > interfaces;

        // Iterator over all interfaces
        for (boost::property_tree::ptree::value_type &interface : rootJson.get_child("interfaces"))
        {
            // Get the label of the node
            string name = interface.first;
            // Get the content of the node
            string color = interface.second.data();
            cout << color << endl;
            interfaces.push_back(make_pair(name, color));
        }
        return MDSUCCESS;
    }
    catch (exception const& e)
    {
      cerr << e.what() << endl;
    }
    return MDERROR;
}

int ippsJson::parse()
{
    try
    {
        string msg = rootJson.get<string>("version");
        cout << msg << endl;

        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, rootJson.get_child("version"))
        {
          assert(v.first.empty()); // array elements have no names
          cout << v.first.data() << endl;
          // etc
        }
        return MDSUCCESS;
    }
    catch (exception const& e)
    {
        cerr << e.what() << endl;
    }
    return MDERROR;
}

