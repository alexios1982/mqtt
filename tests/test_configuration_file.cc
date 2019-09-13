#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <exception>
#include <iostream>

int main(){

  try{
    boost::property_tree::ptree pt;
    boost::property_tree::read_json("configuration_with_reserved_and_multiple_gates.json", pt);
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
  	std::cout << "Ring child number " << ++i << ": " << ring_child.first << std::endl;
	  BOOST_FOREACH(boost::property_tree::ptree::value_type &anonymous_child, ring_child.second) {
	    std::cout << "\tRoom name: " << (anonymous_child.second).get<std::string>("name") << '\n' ;
	    //some rooms might not have gates
	    boost::property_tree::ptree::const_assoc_iterator it = (anonymous_child.second).find("gate");
	    if( it != anonymous_child.second.not_found() ){
	      auto gate_node = (anonymous_child.second).get_child("gate");
	      BOOST_FOREACH(boost::property_tree::ptree::value_type &gate_child, gate_node) {
		std::cout << "\t\tGate name: " << (gate_child.second).get<std::string>("name") << '\n' ;
		auto gate_devices_node = (gate_child.second).get_child("devices");
		BOOST_FOREACH(boost::property_tree::ptree::value_type &gate_devices_child, gate_devices_node) {
		  std::cout << "\t\t\tGate devices id: " << (gate_devices_child.second).get<std::string>("id_secret_device") << '\n' ;
		}
	      }
	    }
	    else{
	      std::cout << "\t\tGate child node is missing" << std::endl;
	    }
	    auto devices_node = (anonymous_child.second).get_child("devices"); 	    
	    BOOST_FOREACH(boost::property_tree::ptree::value_type &devices_child, devices_node) {
	      std::cout << "\t\tDevices name: " << (devices_child.second).get<std::string>("name") << '\n' ;
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


  typedef std::string Sensor_cam_name;
  typedef std::string Cam_name;

  typedef std::pair<Sensor_cam_name, Cam_name> Sensor_cam_pair;
  Sensor_cam_pair sensor_cam_pair;
  std::vector<Sensor_cam_pair> sensor_cam_pairs;
  std::string base_path{"/home/pi/gstreamer/multifiles_saving/"}; 
  try{
    boost::property_tree::ptree pt;
    boost::property_tree::read_json("configuration_with_reserved_and_multiple_gates.json", pt);
    //let's retrieve ring node: external, internal and reserved
    auto ring_node = pt.get_child("ring");
    int i = 0;
    //let's iterate over the ring subnodes
    BOOST_FOREACH(boost::property_tree::ptree::value_type &ring_child, ring_node) {
      std::cout << "Ring child number " << ++i << ": " << ring_child.first << std::endl;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &anonymous_child, ring_child.second) {
	std::cout << "\tRoom name: " << (anonymous_child.second).get<std::string>("name") << '\n' ;
	//some rooms might not have gates
	boost::property_tree::ptree::const_assoc_iterator it = (anonymous_child.second).find("gate");
	if( it != anonymous_child.second.not_found() ){
	  auto gate_node = (anonymous_child.second).get_child("gate");
	  BOOST_FOREACH(boost::property_tree::ptree::value_type &gate_child, gate_node) {
	    std::cout << "\t\tGate name: " << (gate_child.second).get<std::string>("name") << '\n' ;
	    auto gate_devices_node = (gate_child.second).get_child("devices");
	    BOOST_FOREACH(boost::property_tree::ptree::value_type &gate_devices_child, gate_devices_node) {
	      std::cout << "\t\t\tGate devices id: " << (gate_devices_child.second).get<std::string>("id_secret_device") << '\n' ;
	      if( (gate_devices_child.second).get<int>("sensor_type") == 3 )
		sensor_cam_pair.second = base_path + (gate_devices_child.second).get<std::string>("id_secret_device");
	      else
		sensor_cam_pair.first = (gate_devices_child.second).get<std::string>("id_secret_device");		  
	    }
	  }
	  sensor_cam_pairs.push_back(sensor_cam_pair);
	}
	else{
	  std::cout << "\t\tGate child node is missing" << std::endl;
	}
      }
    }//END_OF BOOST_FOR_EACH
  }//END OF INNER TRY
  catch(std::exception &e){
    std::cout << "Error: " << e.what() << "\n";
  }

  std::cout << "\n\n" ;
  for(auto item: sensor_cam_pairs)
    std::cout << "sensor: " << item.first << "   cam: " << item.second << std::endl; 
      
  return 0;
}
