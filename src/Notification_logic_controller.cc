#include <sstream>
#include "Notification_logic_controller.hh"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define DEBUG

Notification_logic_controller::Notification_logic_controller(Area_protection &area_protection,
							     Publisher &publisher,
							     std::map<std::string, std::string> &sensor_cam,
							     std::map<std::string, std::string> &cam_path):
  _area_protection{area_protection},
  _publisher{publisher},
  _sensor_cam{sensor_cam},
  _cam_path{cam_path}{}

void Notification_logic_controller::consume_message(){
  while(true){
    mqtt::const_message_ptr msg =  _queue.get();
    classify_message(msg);
  }
}

void Notification_logic_controller::classify_message(const mqtt::const_message_ptr &message_ptr){
  //Let's retrieve the topic to undertand what to do
  std::string topic = message_ptr->get_topic();
  if (!topic.compare(26,4, "99b3")){
    D(std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << "is the door sensor"<<'\n');
    //let's check that it is not a duplicate sesnsor signal
    if( is_a_door_sensor_notification_duplicate(message_ptr) )
      //nothing to do
      return;
    //let's prepare the message and send
    mqtt::const_message_ptr message_to_send = prepare_classified_notification(message_ptr);
    send_notification(message_to_send);
  }
}

void Notification_logic_controller::update_area_protection(){
  _area_protection.update();
}

bool Notification_logic_controller::is_a_door_sensor_notification_duplicate(const mqtt::const_message_ptr &message_ptr){
    //solution to double notification problems
    //we discard the second message because sensor port always
    //sends two messages for each event: closed door and opened door
    static int previous_contact = -1;
    std::string payload( message_ptr->to_string() );
    std::stringstream ss;
    
    D(std::cout << warning << "[Subscriber_action_listener::" << __func__ << "]. "  << reset << "payload: '" << payload << '\n';)
    ss << payload;

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);
    //contact value is true or false, but we want to convert to an int value
    int actual_contact = pt.get<bool>("contact");
    std::cout << "actual contact: " << actual_contact << '\n';	
    if(actual_contact != previous_contact){
      return false;
      previous_contact = actual_contact;
    }
    return true;
}

mqtt::const_message_ptr Notification_logic_controller::prepare_rich_notification(const mqtt::const_message_ptr &message_ptr){

}

mqtt::const_message_ptr Notification_logic_controller::prepare_classified_notification(const mqtt::const_message_ptr &message_ptr){

}

void Notification_logic_controller::send_notification(const mqtt::const_message_ptr &message_ptr){

}
