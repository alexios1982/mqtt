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

  //std::string configuration_file{"configuration_integration_test.json"};
  std::string configuration_file{"test_configuration_with_reserved_and_multiple_gates.json"};
  
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

  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " number of levels: " << notification_logic_controller.get_number_of_levels() << '\n'; 

  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " printing _motion_sensors_maps" << '\n'; 
  for(auto outer_value : notification_logic_controller._motion_sensors_maps){
    std::cout << outer_value.first << " -> " << outer_value.second << '\n';
    for( auto inner_value : *(outer_value.second) )
      std::cout << '\t' << inner_value.first << " -> " << std::boolalpha << inner_value.second << '\n';
  }

  std::string extern_motion_sensor{"0x20_motion"};
  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " is ring associated to the sensor: " << extern_motion_sensor << " occupied: " << notification_logic_controller.is_ring_occupied(extern_motion_sensor) << '\n';

  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " setting to true " << extern_motion_sensor << '\n';
  notification_logic_controller.update_motion_sensor_state(extern_motion_sensor, true);
  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " is ring associated to the sensor: " << extern_motion_sensor << " occupied: " << notification_logic_controller.is_ring_occupied(extern_motion_sensor) << '\n';

  std::string extern_motion_sensor_2{"0x22_motion"};
  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " setting to true " << extern_motion_sensor_2 << '\n';
  notification_logic_controller.update_motion_sensor_state(extern_motion_sensor_2, true);
  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " is ring associated to the sensor_2: " << extern_motion_sensor_2 << " occupied: " << notification_logic_controller.is_ring_occupied(extern_motion_sensor_2) << '\n';

  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " setting to false " << extern_motion_sensor_2 << '\n';
  notification_logic_controller.update_motion_sensor_state(extern_motion_sensor_2, false);
  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " is ring associated to the sensor: " << extern_motion_sensor << " occupied: " << notification_logic_controller.is_ring_occupied(extern_motion_sensor) << '\n';
  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " is ring associated to the sensor_2: " << extern_motion_sensor_2 << " occupied: " << notification_logic_controller.is_ring_occupied(extern_motion_sensor_2) << '\n';

  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " setting to false " << extern_motion_sensor << '\n';
  notification_logic_controller.update_motion_sensor_state(extern_motion_sensor, false);
  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " is ring associated to the sensor: " << extern_motion_sensor << " occupied: " << notification_logic_controller.is_ring_occupied(extern_motion_sensor) << '\n';
  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " is ring associated to the sensor: " << extern_motion_sensor_2 << " occupied: " << notification_logic_controller.is_ring_occupied(extern_motion_sensor_2) << '\n';

  return 0;
}
