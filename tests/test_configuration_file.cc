#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <exception>
#include <iostream>
#include <map>

typedef std::string Sensor_mini_id;
typedef std::string Cam_directory;
typedef std::map<Sensor_mini_id, Cam_directory> Sensor_cam_path;

enum class Sensor_type{
  DOOR,
  MOTION,
  WINDOW,
  CAM
};


void fill_sensor_cam_path(Sensor_cam_path &to_fill){
  typedef std::pair<Sensor_mini_id, Cam_directory> Sensor_cam_pair;
  Sensor_cam_pair sensor_cam_pair;
  std::string base_path{"/home/pi/gstreamer/multifiles_saving/"};
    try{
    boost::property_tree::ptree pt;
    boost::property_tree::read_json("configuration_with_reserved_and_multiple_gates.json", pt);
    //int i = 0;
    //let's retrieve the ptree under the ring path
    //each ptree is an iterable object: iterating over it returns a pair (boost::property_tree::ptree::value_type)
    //in wich the first member is the name of the child and the second is the child tree
    //in this case the ptrees under ring are external, internal and reserved
    for( const auto &ring_child : pt.get_child("ring") ){
      //std::cout << "Ring child number " << ++i << ": " << ring_child.first << std::endl;
      //ring_child.second are the trees under external, internal and reserved
      for(const auto &anonymous_child : ring_child.second) {
	//std::cout << "\tRoom name: " << (anonymous_child.second).get<std::string>("name") << '\n' ;
	//we are looking for gates in the room
	//not all rooms may have gates: for this reason we must test its presence
	boost::property_tree::ptree::const_assoc_iterator it = (anonymous_child.second).find("gate");
	if( it != anonymous_child.second.not_found() ){
	  for(const auto &gate_child : (anonymous_child.second).get_child("gate")) {
	    //std::cout << "\t\tGate name: " << (gate_child.second).get<std::string>("name") << '\n' ;
	    auto gate_devices_node = (gate_child.second).get_child("devices");
	    for(const auto &gate_devices_child : gate_devices_node) {
	      //std::cout << "\t\t\tGate devices id: " << (gate_devices_child.second).get<std::string>("id_secret_device") << '\n' ;
	      //if sensor_type == 3 is a cam, otherwise is the sensor that triggers the cam
	      if( (gate_devices_child.second).get<int>("sensor_type") == 3 )
		sensor_cam_pair.second = base_path + (gate_devices_child.second).get<std::string>("id_secret_device");
	      else
		sensor_cam_pair.first = (gate_devices_child.second).get<std::string>("id_secret_device");		  
	    }
	  }
	  to_fill.insert(sensor_cam_pair);
	}
	else{
	  std::cerr << "\t\tGate is missing" << std::endl;
	}
      }
    }//END_OF BOOST_FOR_EACH
  }//END OF INNER TRY
  catch(std::exception &e){
    std::cerr << "Error: " << e.what() << "\n";
  }
}

typedef std::vector<Sensor_mini_id> Sensor_mini_ids;

void fill_devices_events_map(const std::string &ring_type,
			     Sensor_type sensor_type,
			     Sensor_mini_ids &sensor_mini_ids){
  std::string complete_path{std::string{"ring."} + ring_type};
  boost::property_tree::ptree pt;
  boost::property_tree::read_json("configuration_with_reserved_and_multiple_gates.json", pt);
  for(const auto &anonymous_child : pt.get_child(complete_path)) {
    std::cout << "\tRoom name: " << (anonymous_child.second).get<std::string>("name") << '\n' ;
    //we are looking for gates in the room
    //not all rooms may have gates: for this reason we must test its presence
    boost::property_tree::ptree::const_assoc_iterator it = (anonymous_child.second).find("gate");
    if( it != anonymous_child.second.not_found() ){
      std::cout << "\t\tGate exists\n" ;
      for(const auto &gate_child : (anonymous_child.second).get_child("gate")) {
	//std::cout << "\t\tGate name: " << (gate_child.second).get<std::string>("name") << '\n' ;
	auto gate_devices_node = (gate_child.second).get_child("devices");
	for(const auto &gate_devices_child : gate_devices_node) {
	  if( static_cast<Sensor_type>( (gate_devices_child.second).get<int>("sensor_type") ) == sensor_type )
	    sensor_mini_ids.push_back( (gate_devices_child.second).get<std::string>("id_secret_device") ); 	    
	}
      }
    }
    else{
      std::cerr << "\t\tGate is missing" << std::endl;
    }	
    for(const auto &devices_child : (anonymous_child.second).get_child("devices") ){
      // sensor_type_map.insert(std::make_pair((devices_child.second).get<std::string>("id_secret_device"),
      // 					    static_cast<Sensor_type>( (devices_child.second).get<int>("sensor_type") )				    )
      // 			     );
      if(static_cast<Sensor_type>( (devices_child.second).get<int>("sensor_type") ) ==
	 sensor_type)
	sensor_mini_ids.push_back( (devices_child.second).get<std::string>("id_secret_device") ); 
    }
  }
}

int main(){

  std::cout << "\n\nCalling fill_sensor_cam_path" << '\n';
  Sensor_cam_path sensor_cam_path;
  fill_sensor_cam_path(sensor_cam_path);
  for(auto item : sensor_cam_path)
    std::cout << "sensor: " << item.first << "   cam: " << item.second << std::endl; 

  std::cout << "\n\nCalling fill_devices_events_map" << '\n';

  std::cout << "filtering internal motion sensor" << '\n';
  Sensor_mini_ids sensor_mini_ids;
  fill_devices_events_map("internal", Sensor_type::MOTION, sensor_mini_ids);
  for(auto item : sensor_mini_ids)
    std::cout << item << std::endl; 

  sensor_mini_ids.clear();
  std::cout << "filtering internal window sensor" << '\n';
  fill_devices_events_map("internal", Sensor_type::WINDOW, sensor_mini_ids);
  for(auto item : sensor_mini_ids)
    std::cout << item << std::endl; 

  sensor_mini_ids.clear();
  std::cout << "filtering reserved window sensor" << '\n';
  fill_devices_events_map("reserved", Sensor_type::WINDOW, sensor_mini_ids);
  for(auto item : sensor_mini_ids)
    std::cout << item << std::endl; 

  sensor_mini_ids.clear();
  std::cout << "filtering internal door sensor" << '\n';
  fill_devices_events_map("internal", Sensor_type::DOOR, sensor_mini_ids);
  for(auto item : sensor_mini_ids)
    std::cout << item << std::endl; 

  sensor_mini_ids.clear();
  std::cout << "filtering reserved door sensor" << '\n';
  fill_devices_events_map("reserved", Sensor_type::DOOR, sensor_mini_ids);
  for(auto item : sensor_mini_ids)
    std::cout << item << std::endl; 

  return 0;
}
