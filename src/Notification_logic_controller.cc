#include <sstream>
#include "Notification_logic_controller.hh"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "base64.hh"

#define DEBUG

Notification_logic_controller::Notification_logic_controller(Area_protection &area_protection,
							     Synchronized_queue<mqtt::const_message_ptr> &queue,
							     Publisher &publisher,
							     std::map<std::string, std::string> &sensor_cam,
							     std::map<std::string, std::string> &cam_path):
  _area_protection{area_protection},
  _queue{queue},
  _publisher{publisher},
  _sensor_cam{sensor_cam},
  _cam_path{cam_path}{}

void Notification_logic_controller::consume_message(){
  while(true){
    mqtt::const_message_ptr msg =  _queue.get();
    //the following is a blocking operation till these
    //sent of the messages
    //classify_message(msg);
    std::thread(&Notification_logic_controller::classify_message, this, msg).detach();
  }
}

void Notification_logic_controller::classify_message(const mqtt::const_message_ptr &message_ptr){
  // //Let's retrieve the topic to undertand what to do
  // std::string topic = message_ptr->get_topic();
  // if (!topic.compare(26,4, "99b3")){
  //   D(std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << "is the door sensor"<<'\n');
  //   //let's check that it is not a duplicate sensor signal
  //   if( is_a_door_sensor_notification_duplicate(message_ptr) )
  //     //nothing to do
  //     return;
  //   //let's prepare the message and send
  //   mqtt::const_message_ptr message_to_send = prepare_classified_notification(message_ptr);
  //   send_notification(message_to_send);
  // }
  //topic is in the form of zigbee2mqtt/0x00158d0001cc99b3
  //and we want to extract only the last 8 digits
  std::string topic = message_ptr->get_topic();
  std::string topic_info = topic.substr(topic.size() - 8);
  std::cout << warning << "[Notification_logic_controller::" << __func__ << "] " << reset
     << "topic_info is: " << topic_info << '\n';
  //let's do a search in the _sensor_cam map to decide if we have
  //to prepare a rich notification or a classified notification
  mqtt::const_message_ptr message_to_send;
  auto it = _sensor_cam.find(topic_info);
  if( it != _sensor_cam.end() ){
    if( !is_a_door_sensor_notification_duplicate(message_ptr) ){
      int iter = 0, n_of_sending = 2;
      while( iter < n_of_sending){
	message_to_send = prepare_rich_notification(message_ptr, topic_info);
	send_notification(message_to_send);
	++iter;
      }
    }
    //nothing to do: the associate message has already been sent
    else
      return;
  }
  //if it's not in the sensor_cam map, it could be a rich message or
  //an answer from the ai serder (in this case the topic is Response
  else if (topic_info == "Response"){
    analyze_ai_response(message_ptr);
  }
  //it is a sensor associate to a classified notification
  else{
    message_to_send = prepare_classified_notification(message_ptr);
    //send_notification(message_to_send);
  }
}

mqtt::const_message_ptr Notification_logic_controller::prepare_rich_notification(const mqtt::const_message_ptr &message_ptr, const std::string &sensor_mini_id){
  boost::property_tree::ptree pt;
  Dir_handler dir_handler{ _cam_path[ _sensor_cam[sensor_mini_id] ] };
  if( !dir_handler.exists() ){
    std::cerr << "[Notification_logic_controller::" << __func__ << "]. " << "no directory with mp4 file. " << std::endl; 
    exit(-1);
  }
  Dir_handler::Time_path_pair to_send = dir_handler.get_last_modified_file(".mp4");
  std::string to_send_filename = ( (to_send.second).filename() ).string();
  std::time_t now;
  std::time (&now);
  std::cout << warning << "[Notification_logic_controller::" << __func__ << "] " << reset
     << "The current local time is: " << std::ctime(&now)  << '\n';
     std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset
     << "video " /*<< (iter + 1) <<*/" to publish is " << to_send_filename << '\n';
  //let's wait until the video chunk to send is finished:
  //the condition for this to happen is the presence of a next video chunk 
  while(true){
    Dir_handler::Time_path_pair next = dir_handler.get_last_modified_file(".mp4");
    if( ( (next.second).filename() ).string() != to_send_filename )
      break;
    std::this_thread::sleep_for ( std::chrono::milliseconds(500) ); 
  }
  
  pt.put("filename", to_send_filename);
  pt.put( "data", base64_file_converter( (to_send.second).string() ) );
  
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, pt);
  return mqtt::make_message( _publisher.get_topic(), ss.str() );
  
}

mqtt::const_message_ptr Notification_logic_controller::prepare_classified_notification(const mqtt::const_message_ptr &message_ptr){
  std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << '\n';
  return mqtt::make_message("", "");
}

void Notification_logic_controller::send_notification(const mqtt::const_message_ptr &message_ptr){
  std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << "publishing ..." << '\n';
  _publisher.publish(message_ptr);
}

 bool Notification_logic_controller::is_a_door_sensor_notification_duplicate(const mqtt::const_message_ptr &message_ptr){
    //solution to double notification problems
    //we discard the second message because sensor port always
    //sends two messages for each event: closed door and opened door
    static int previous_contact = -1;
    std::string payload( message_ptr->to_string() );
    std::stringstream ss;
    
    std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. "  << reset << "payload: '" << payload << '\n';
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

 void Notification_logic_controller::update_area_protection(){
  _area_protection.update();
}

void Notification_logic_controller::analyze_ai_response(const mqtt::const_message_ptr &message_ptr){
  std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << "topic: " << message_ptr->get_topic() << '\n';
  std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << "payload: " << message_ptr->get_payload() << '\n';  
  
}
