#ifndef _NOTIFICATION_LOGIC_CONTROLLER_
#define _NOTIFICATION_LOGIC_CONTROLLER_

#include "Area_protection.hh"
#include "Synchronized_queue.hh"
#include "Publisher.hh"
#include <map>

class Notification_logic_controller{
  Area_protection &_area_protection;
  Synchronized_queue<mqtt::const_message_ptr> &_queue;
  Publisher &_publisher;
  std::map<std::string, std::string> &_sensor_cam;
  std::map<std::string, std::string> &_cam_path;

  ///this method is called by consume_message when a message is present in the queue
  ///check the message type and call the appropriate handlers
  void classify_message(const mqtt::const_message_ptr &zigbee_message_ptr);
  ///the door sensor sent two messages associate to the same event
  ///this metod is used to recognize this case
  bool is_a_door_sensor_notification_duplicate(const mqtt::const_message_ptr &zigbee_message_ptr);
  ///handler called by classify_message when the message present in the queue is
  ///associated to a sensor that requests a rich notification
  mqtt::const_message_ptr prepare_rich_notification(const mqtt::const_message_ptr &zigbee_message_ptr,
						    const std::string &sensor_mini_id);
  ///handler called by classify_message when the message present in the queue is
  ///associated to a sensor that requests a classified notification  
  mqtt::const_message_ptr prepare_classified_notification(const mqtt::const_message_ptr &zigbee_message_ptr);
  ///publish the notification
  void send_notification(const mqtt::const_message_ptr &message_ptr);
  ///handler called by classify_message when the message present in the queue is
  ///received by the ai server 
  void analyze_ai_response(const mqtt::const_message_ptr &message_ptr);
public:
  Notification_logic_controller(Area_protection &area_protection,
				Synchronized_queue<mqtt::const_message_ptr> &queue,
				Publisher &publisher,
				std::map<std::string, std::string> &sensor_cam,
				std::map<std::string, std::string> &cam_path);
  ///this method check the presence of a message in the queue and start
  ///classify_message in a thread to take the required actions
  void consume_message();
  void update_area_protection();
};
#endif
