#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <exception>
#include <iostream>
#include <map>
#include "Notification_logic_controller.hh"
#include <boost/core/ignore_unused.hpp>

int main(int argc, char **argv){
  boost::ignore_unused(argc);
  boost::ignore_unused(argv);

  std::string configuration_file{"configuration_integration_test.json"};
  
  Area_protection fake_area_protection;
  Synchronized_queue<mqtt::const_message_ptr> fake_queue;
  mqtt::async_client fake_remote_publisher("", "");
  Publisher_callback fake_publisher_callback;
  fake_remote_publisher.set_callback(fake_publisher_callback);

  Delivery_action_listener fake_publisher_listener;
  Dir_handler fake_dir_handler{""};
  Publisher fake_publisher(fake_remote_publisher,
		      "",
		      fake_publisher_listener,
		      fake_queue,
		      fake_dir_handler);

  Notification_logic_controller notification_logic_controller{fake_area_protection, fake_queue, fake_publisher};

  notification_logic_controller.load_configuration(configuration_file);

  for(auto item : notification_logic_controller._sensor_cam_path)
    std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " sensor: " << item.first << "   cam: " << item.second << '\n'; 
  
  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " printing sensor_infos_map" << '\n'; 
  for(auto item : notification_logic_controller._sensor_infos_map)
    std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " sensor id: " << item.first << " -> sensor type: " << static_cast<int>( (item.second)._sensor_type ) << " sensor position: " << (item.second)._position << '\n'; 


  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " printing sensor_infos_map_2" << '\n'; 
  for(auto item : notification_logic_controller._sensor_infos_map_2)
    std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " sensor id: " << item.first << " -> sensor type: " << static_cast<int>( (item.second)._sensor_type ) << " sensor position: " << (item.second)._position << '\n'; 

  return 0;
}
