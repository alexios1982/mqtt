#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <exception>
#include <iostream>

int main(){

  try{
    boost::property_tree::ptree pt;
    boost::property_tree::read_json("configuration_with_reserved.json", pt);
    std::cout << "Success\n";
    std::cout << pt.get<std::string>("address") << '\n';
    std::cout << pt.get<std::string>("city") << '\n';
    std::cout << pt.get<std::string>("control_unit") << '\n';
    std::cout << pt.get<std::string>("name") << '\n';
    try{
      //let's retrieve ring node: external, internal and reserved
      auto ring_node = pt.get_child("ring");
      int i = 0;
      //let's iterate over the ring subnodes
      BOOST_FOREACH(boost::property_tree::ptree::value_type &ring_child, ring_node) {
  	std::cout << "ring child number " << ++i << ": " << ring_child.first << std::endl;
	  BOOST_FOREACH(boost::property_tree::ptree::value_type &anonymous_child, ring_child.second) {
	    std::cout << "\troom name: " << (anonymous_child.second).get<std::string>("name") << '\n' ;
	    auto gate_node = (anonymous_child.second).get_child("gate"); 	    
	    BOOST_FOREACH(boost::property_tree::ptree::value_type &gate_child, gate_node) {
	      std::cout << "\t\tgate name: " << (gate_child.second).get<std::string>("name") << '\n' ;
	      auto gate_devices_node = (gate_child.second).get_child("devices");
	      BOOST_FOREACH(boost::property_tree::ptree::value_type &gate_devices_child, gate_devices_node) {
		std::cout << "\t\t\tgate devices name: " << (gate_devices_child.second).get<std::string>("name") << '\n' ;
	      }
	    }
	    auto devices_node = (anonymous_child.second).get_child("devices"); 	    
	    BOOST_FOREACH(boost::property_tree::ptree::value_type &devices_child, devices_node) {
	      std::cout << "\t\tdevices name: " << (devices_child.second).get<std::string>("name") << '\n' ;
	    }	    
	  }
      }//END_OF BOOST_FOR_EACH
    }//END OF INNER TRY
    catch(std::exception &e){
      std::cout << "Error: " << e.what() << "\n";
    }
  }
  catch (std::exception &e){
    std::cout << "Error: " << e.what() << "\n";
  }

  return 0;
}
