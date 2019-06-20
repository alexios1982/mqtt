#define DEBUG

#include <sstream>
#include "Notification_logic_controller.hh"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "base64.hh"
#include <boost/core/ignore_unused.hpp>

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
  //topic is in the form of zigbee2mqtt/0x00158d0001cc99b3
  //and we want to extract only the last 8 digits
  std::string topic = message_ptr->get_topic();
  std::string topic_info = topic.substr(topic.size() - 8);
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "] " << reset
    << "topic_info is: " << topic_info << '\n';)
  //let's do a search in the _sensor_cam map to decide if we have
  //to prepare a rich notification or a classified notification
  mqtt::const_message_ptr message_to_send;
  auto it = _sensor_cam.find(topic_info);
  if( it != _sensor_cam.end() ){
    if( !is_a_door_sensor_notification_duplicate(message_ptr) ){
      int iter = 0, n_of_sending = 3;
      while( iter < n_of_sending){
	message_to_send = prepare_rich_notification(message_ptr, topic_info);
	D(std::cout << info << "[Notification_logic_controller::" << __func__ << "] " << reset
	  << "message_to_send size is: " << ( message_to_send->to_string() ).size() << '\n';)
	if(message_to_send->get_topic() != "")
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
  static std::string last_sent_short_filename{};
  boost::ignore_unused(message_ptr);
  boost::property_tree::ptree pt;
  Dir_handler dir_handler{ _cam_path[ _sensor_cam[sensor_mini_id] ] };
  if( !dir_handler.exists() ){
    std::cerr << "[Notification_logic_controller::" << __func__ << "]. " << "no directory with mp4 file. " << std::endl; 
    exit(-1);
  }
  std::time_t now;
  std::time (&now);
  D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
     << "The current local time is: " << std::ctime(&now) ); //ctime adds automatically a \n
  Dir_handler::Time_path_pair to_send = dir_handler.get_last_modified_file(".mp4");
  std::string to_send_filename = ( (to_send.second).filename() ).string();
  if( !( to_send_filename.empty() ) ){
    D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
       << "Video " /*<< (iter + 1) <<*/" to publish is " << to_send_filename << std::endl );
    //we take only the last 6 chars because the filename is very
    //long and the comparison between the entire filename can be heavy
    std::string curr_short_filename = to_send_filename.substr(to_send_filename.size() - 6);
    D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
       << "curr: " << curr_short_filename << " last: " << last_sent_short_filename << std::endl);
    if(curr_short_filename != last_sent_short_filename){
      last_sent_short_filename = curr_short_filename;
      //let's wait until the video chunk to send is finished:
      //the condition for this to happen is the presence of a next video chunk 
      while(true){
	Dir_handler::Time_path_pair next = dir_handler.get_last_modified_file(".mp4");
	if( ( (next.second).filename() ).string() != to_send_filename )
	  break;
	std::this_thread::sleep_for ( std::chrono::milliseconds(200) ); 
      }
  
      pt.put("filename", to_send_filename);
      pt.put( "data", base64_file_converter( (to_send.second).string() ) );
  
      std::stringstream ss;
      boost::property_tree::json_parser::write_json(ss, pt);
      //if we put assignment here, we have double sending of the same file
      //last_sent_short_filename = curr_short_filename;
      D( std::cout << error << "[Notification_logic_controller::" << __func__ << "]. " << reset
	 << "filename: " << to_send_filename << " time: " << std::ctime(&now) << std::endl);
      return mqtt::make_message( _publisher.get_topic(), ss.str() );
    }else{
      D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
	 << "Video alredy sent" << std::endl );
      return mqtt::make_message("", "");
    }
  }
  else{
    std::cerr << error << "[Notification_logic_controller::" << __func__ << "]. " << reset
	      <<"Video to publish is an empty string: sending a null message" << std::endl;
    return mqtt::make_message("", "");
  }
}

mqtt::const_message_ptr Notification_logic_controller::prepare_classified_notification(const mqtt::const_message_ptr &message_ptr){
  boost::ignore_unused(message_ptr);
  D(std::cout << info << "[Notification_logic_controller::"
    << __func__ << "]. " << reset << '\n';)
  return mqtt::make_message("", "");
}

void Notification_logic_controller::send_notification(const mqtt::const_message_ptr &message_ptr){
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset << "publishing ..." << '\n');
  _publisher.publish(message_ptr);
}

bool Notification_logic_controller::is_a_door_sensor_notification_duplicate(const mqtt::const_message_ptr &message_ptr){
  //solution to double notification problems
  //we discard the second message with the same value of contact tag
  //because sensor port always
  //sends two messages for each event: closed door and opened door
  static int previous_contact = -1;
  std::string payload( message_ptr->to_string() );
  std::stringstream ss;
   
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << "payload: '" << payload << '\n';)
  ss << payload;
   
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);
  //contact value is true or false, but we want to convert to an int value
  int actual_contact = pt.get<bool>("contact");
  //I tried to put #undef DEBUG here to avoid printing but it doesn't work
#if 0
  D(std::cout << info << "Notification_logic_controller::" << "] " << reset <<
    "actual contact: " << actual_contact << '\n';)
#endif
  if(actual_contact != previous_contact){
    previous_contact = actual_contact;
    return false;
  }
  return true;
}

void Notification_logic_controller::update_area_protection(){
  _area_protection.update();
}

void Notification_logic_controller::analyze_ai_response(const mqtt::const_message_ptr &message_ptr){
  D(std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << "topic: " << message_ptr->get_topic() << '\n');
  D(std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << "payload: " << message_ptr->get_payload() << std::endl);
  std::time_t now;
  std::time (&now);
  D( std::cout << error << "[Notification_logic_controller::" << __func__ << "]. " << reset
     << " time: " << std::ctime(&now) << std::endl);
  
}
