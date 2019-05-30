#ifndef _NOTIFICATION_LOGIC_CONTROLLER_
#define _NOTIFICATION_LOGIC_CONTROLLER_

#include "Area_protection.hh"
#include "Synchronized_queue.hh"
#include "Publisher.hh"
#include <map>

class Notification_logic_controller{
  Area_protection &_area_protection;
  Synchronized_queue<mqtt::const_message_ptr> _queue;
  Publisher &_publisher;
  std::map<std::string, std::string> &_sensor_cam;
  std::map<std::string, std::string> &_cam_path;
  
  void classify_message(const mqtt::const_message_ptr &message_ptr);
  bool is_a_door_sensor_notification_duplicate(const mqtt::const_message_ptr &message_ptr);
  mqtt::const_message_ptr prepare_rich_notification(const mqtt::const_message_ptr &message_ptr);
  mqtt::const_message_ptr prepare_classified_notification(const mqtt::const_message_ptr &message_ptr);
  void send_notification(const mqtt::const_message_ptr &message_ptr);
public:
  Notification_logic_controller(Area_protection &area_protection,
				Publisher &publisher,
				std::map<std::string, std::string> &sensor_cam,
				std::map<std::string, std::string> &cam_path);
  void consume_message();
  void update_area_protection();
};
#endif
