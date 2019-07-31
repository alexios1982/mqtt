#define DEBUG

#include <sstream>
#include "Notification_logic_controller.hh"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "base64.hh"
#include <boost/core/ignore_unused.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


Notification_logic_controller::Notification_logic_controller(Area_protection &area_protection,
							     Synchronized_queue<mqtt::const_message_ptr> &queue,
							     Publisher &publisher,
							     std::map<std::string, std::string> &sensor_cam,  
							     std::map<std::string, std::string> &cam_path,
							     const int JPEG_QUALITY,
							     const int NUMBER_OF_FRAMES_TO_SEND):
  _area_protection{area_protection},
  _queue{queue},
  _publisher{publisher},
  _sensor_cam{sensor_cam},
  _cam_path{cam_path},
  _number_of_sent_frames{0},
  _number_of_received_responses{0},
  _NUMBER_OF_FRAMES_TO_SEND{NUMBER_OF_FRAMES_TO_SEND}{
    _jpeg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    _jpeg_params.push_back(JPEG_QUALITY);
  }

void Notification_logic_controller::consume_message(){
  while(true){
    mqtt::const_message_ptr msg =  _queue.get();
    //the following is a blocking operation till these
    //sent of the messages
    //classify_message(msg);
    std::thread(&Notification_logic_controller::classify_message, this, msg).detach();
  }
}

// void Notification_logic_controller::classify_message(const mqtt::const_message_ptr &zigbee_message_ptr){
//   //topic is in the form of zigbee2mqtt/0x00158d0001cc99b3
//   std::string topic = zigbee_message_ptr->get_topic();
//   //and we want to extract only the last 8 digits
//   std::string topic_info = topic.substr(topic.size() - 8);
//   D(std::cout << info << "[Notification_logic_controller::" << __func__ << "] " << reset
//     << "topic_info is: " << topic_info << '\n');
//   //let's do a search in the _sensor_cam map to decide if we have
//   //to prepare a rich notification or a classified notification
//   mqtt::const_message_ptr message_to_send;
//   auto it = _sensor_cam.find(topic_info);
//   if( it != _sensor_cam.end() ){
//     if( !is_a_door_sensor_notification_duplicate(zigbee_message_ptr) ){
//       D( Time_spent<> to_send_notifications );
//       // int iter = 0, n_of_sending = 3;
//       // while( iter < n_of_sending){
//       // 	message_to_send = prepare_rich_notification(zigbee_message_ptr, topic_info);
//       // 	D(std::cout << info << "[Notification_logic_controller::" << __func__ << "] " << reset
//       // 	  << "message_to_send size is: " << ( message_to_send->to_string() ).size() << '\n';)
//       // 	if(message_to_send->get_topic() != "")
//       // 	  send_notification(message_to_send);
//       // 	++iter;
//       // }
//       send_rich_notifications(topic_info, 1, 2);
//     }
//     //nothing to do: the associate message has already been sent
//     else
//       return;
//   }
//   //if it's not in the sensor_cam map, it could be a rich message or
//   //an answer from the ai serder (in this case the topic is Response
//   else if (topic_info == "Response"){
//     analyze_ai_response(zigbee_message_ptr);
//   }
//   //it is a sensor associate to a classified notification
//   else{
//     message_to_send = prepare_classified_notification(zigbee_message_ptr);
//     //send_notification(message_to_send);
//   }
// }

void Notification_logic_controller::classify_message(const mqtt::const_message_ptr &zigbee_message_ptr){
  //topic is in the form of zigbee2mqtt/0x00158d0001cc99b3
  std::string topic = zigbee_message_ptr->get_topic();
  //and we want to extract only the last 8 digits
  std::string topic_info = topic.substr(topic.size() - 8);
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "] " << reset
    << "topic_info is: " << topic_info << '\n');
  //let's do a search in the _sensor_cam map to decide if we have
  //to prepare a rich notification or a classified notification
  mqtt::const_message_ptr message_to_send;

  //auto it = _sensor_cam.find(topic_info);
  auto it = _sensor_proc_events_map.find(topic_info);
  if( it != _sensor_proc_events_map.end() ){
    switch(_sensor_type_map[topic_info]){
    case Sensor_type::CONTACT :
      if( is_gate_opened(zigbee_message_ptr) ){
	D( Time_spent<> to_send_notifications );
	(it->second)();
      }
      //nothing to do: the associate message has already been sent
      else
	return;
      break;
    case Sensor_type::MOTION :
	D( Time_spent<> to_send_notifications );
	(it->second)();
	break;
    }
  }
  //if it's not in the sensor_proc_events_map, it could be a rich message or
  //an answer from the ai serder (in this case the topic is Response
  else if (topic_info == "Response"){
    analyze_ai_response(zigbee_message_ptr);
  }
}

void Notification_logic_controller::send_rich_notification(const std::string &sensor_mini_id,
							   int which,
							   File_type file_type){
  static std::string last_sent_short_filename{};
  std::time_t now;
  //let's initialize the now varibile with the current time
  std::time (&now);
  D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
     << "The current local time is: " << std::ctime(&now) ); //ctime adds automatically a \n
  std::unique_ptr<Dir_handler::Time_path_pair> to_send_ptr = select_video_chunk(sensor_mini_id, which);
  std::string to_send_filename = ( (to_send_ptr->second).filename() ).string();
  if( !( to_send_filename.empty() ) ){
    //we take only the last 10 chars because the filename is very
    //long and the comparison between the entire filename can be heavy
    std::string curr_short_filename = to_send_filename.substr(to_send_filename.size() - 10);
    D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
       << "curr: " << curr_short_filename << " last: " << last_sent_short_filename << std::endl);
    if(curr_short_filename != last_sent_short_filename){
      last_sent_short_filename = curr_short_filename;
      mqtt::const_message_ptr message_to_send = prepare_rich_notification(to_send_ptr, file_type, sensor_mini_id);
      if(message_to_send->get_topic() != ""){
	      	D(std::cout << info << "[Notification_logic_controller::" << __func__ << "] " << reset
      	  << "message_to_send size is: " << ( message_to_send->to_string() ).size() << '\n';)
	send_notification(message_to_send);
      }
    }
    else{
      D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
	 << "Video alredy sent" << std::endl );
    }
  }else{
    std::cerr << error << "[Notification_logic_controller::" << __func__ << "]. " << reset
	      <<"Video to publish is an empty string: sending a null message" << std::endl;
  }
}

void Notification_logic_controller::send_rich_notifications(const std::string &sensor_mini_id,
							    int which,
							    int how_many_later,
							    File_type file_type){
  //sending first message
  D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
     << "Send the first rich notification" << std::endl );
  {
    D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
     << "time spent for the first message" << std::endl );
    D(Time_spent<> first_message_timer);
    send_rich_notification(sensor_mini_id, which, file_type);
  }
  //sending the others
  D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
     << "Send the other "<< how_many_later << " notifications" << std::endl );
  for(int iter{0}; iter < how_many_later; ++iter)
    send_rich_notification(sensor_mini_id, 0, file_type);
}

mqtt::const_message_ptr Notification_logic_controller::prepare_rich_notification(const std::unique_ptr<Dir_handler::Time_path_pair> &to_send_ptr, File_type file_type, const std::string &sensor_mini_id){
  boost::property_tree::ptree pt;
  std::string to_send_filename = ( (to_send_ptr->second).filename() ).string(); 
  if(file_type == MP4){
    pt.put("filename", to_send_filename );
    pt.put( "data", base64_file_converter( (to_send_ptr->second).string() ) );
    pt.put("position", _sensor_position_map[sensor_mini_id]);
   }
  else if(file_type == JPEG){
    static std::string last_jpeg_file{};
    //let's remove last jpeg file
    std::remove( last_jpeg_file.c_str() );
    pt.put("filename", to_send_filename.replace(to_send_filename.find_last_of('.'), std::string::npos, ".jpeg") );
    cv::VideoCapture cap( (to_send_ptr->second).string() );
    if ( !cap.isOpened() ){
      std::cerr << error << "[Notification_logic_controller::" << __func__ << "]. "
		<< reset << "Cannot open the video file" << std::endl;
      exit(1);
    }
    //let's go to 0.5 second in the video
    cap.set(CV_CAP_PROP_POS_MSEC, 500);
    cv::Mat frame;
    bool b_success = cap.read(frame);
    if ( !b_success) {
      std::cerr << error << "[Notification_logic_controller::" << __func__ << "]. "
		<< reset << "Cannot read the frame from video file" << std::endl;
      exit(1);
    }
    std::string mp4_complete_filename = (to_send_ptr->second).string();
    std::string jpeg_complete_filename( mp4_complete_filename.replace(mp4_complete_filename.find_last_of('.'), std::string::npos, ".jpeg") );
      
    imwrite(jpeg_complete_filename, frame, _jpeg_params);
    
    pt.put( "data", base64_file_converter(jpeg_complete_filename) );
    pt.put("position", _sensor_position_map[sensor_mini_id]);
    last_jpeg_file = jpeg_complete_filename;
  }
  else{
    std::cerr << error << "[Notification_logic_controller::" << __func__ << "]. " << reset
	      << "Unknown file format" << std::endl;
    exit(2);
  }
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, pt);
  //if we put assignment here, we have double sending of the same file
  //last_sent_short_filename = curr_short_filename;
  D( std::cout << error << "[Notification_logic_controller::" << __func__ << "]. " << reset
     << "filename: " << to_send_filename /*<< " time: " << std::ctime(&now)*/ << std::endl);
  return mqtt::make_message( _publisher.get_topic(), ss.str() );
}

std::unique_ptr<Dir_handler::Time_path_pair>
Notification_logic_controller::select_video_chunk(const std::string &sensor_mini_id, int which){
  Dir_handler dir_handler{ _cam_path[ _sensor_cam[sensor_mini_id] ] };
  //this snippet must be executed just once to check that the dir exists
  //to avoid unnecessary execution, we use this trick that uses lambda function
  static bool once = [&dir_handler]{
    std::cout << "just once" << std::endl;
    if( !dir_handler.exists() ){
      std::cerr << "[Notification_logic_controller::" << __func__ << "]. " << "no directory with mp4 file. " << std::endl; 
      exit(1);
    }
    return true;
  }();
  /////////////////////////////////////////////////////////////////////////////////////
  boost::ignore_unused(once);
    
  std::unique_ptr<Dir_handler::Time_path_pair> to_send_ptr{};
  int iter{0};
  while(iter <= which){
    to_send_ptr = std::make_unique<Dir_handler::Time_path_pair>( dir_handler.get_last_modified_file(".mp4") );
    std::string to_send_filename = ( (to_send_ptr->second).filename() ).string();
    //let's wait until the video chunk to send is finished:
    //the condition for this to happen is the presence of a next video chunk
    D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
       << "current filename: " << to_send_filename <<  std::endl);
    while(true){
      Dir_handler::Time_path_pair next = dir_handler.get_last_modified_file(".mp4");
      if( ( (next.second).filename() ).string() != to_send_filename )
	break;
      std::this_thread::sleep_for ( std::chrono::milliseconds(200) ); 
    }
    ++iter;
  }
  return to_send_ptr;
}

mqtt::const_message_ptr Notification_logic_controller::prepare_classified_notification(const mqtt::const_message_ptr &zigbee_message_ptr){
  boost::ignore_unused(zigbee_message_ptr);
  D(std::cout << info << "[Notification_logic_controller::"
    << __func__ << "]. " << reset << '\n';)
    return mqtt::make_message("", "");
}

void Notification_logic_controller::send_notification(const mqtt::const_message_ptr &message_ptr){
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset << "publishing ..." << '\n');
  _publisher.publish(message_ptr);
}

// Notification_logic_controller::Contact_sensor_state
// Notification_logic_controller::check_contact_sensor_state(const mqtt::const_message_ptr &zigbee_message_ptr){
//   //solution to double notification problems
//   //we discard the second message with the same value of contact tag
//   //because sensor port always
//   //sends two messages for each event: closed door and opened door
//   static int previous_contact = -1;
//   std::string payload( zigbee_message_ptr->to_string() );
//   std::stringstream ss;
   
//   D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << "payload: '" << payload << '\n';)
//     ss << payload;
   
//   boost::property_tree::ptree pt;
//   boost::property_tree::read_json(ss, pt);
//   Contact_sensor_state contact_sensor_state{false, false};
//   contact_sensor_state.actual_contact = pt.get<bool>("contact");
//   //contact value is true or false, but we want to convert to an int value
//   int actual_contact = contact_sensor_state.actual_contact;

//   //  //I tried to put #undef DEBUG here to avoid printing but it doesn't work
//   // #if 0
//   //   D(std::cout << info << "Notification_logic_controller::" << "] " << reset <<
//   //     "actual contact: " << actual_contact << '\n';)
//   // #endif
    
//   //if actual_contact is true the door has been closed and we don't have to check
//   //if it a dublicate because in this case no event will be triggered
//   if(contact_sensor_state.actual_contact)
//     return contact_sensor_state;
  
//   //if actual_contact is false the door has been open and we have to check
//   //if it a dublicate because in this case no event will be trigger
//   if(actual_contact != previous_contact){
//     previous_contact = actual_contact;
//     contact_sensor_state.is_a_duplicate = false;
//   }
//   else
//     contact_sensor_state.is_a_duplicate = true;

//   return contact_sensor_state;
// }

Notification_logic_controller::Is_gate_opened
Notification_logic_controller::is_gate_opened(const mqtt::const_message_ptr &zigbee_message_ptr){
  std::string payload( zigbee_message_ptr->to_string() );
  std::stringstream ss;
   
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << "payload: '" << payload << '\n');
  ss << payload;
   
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);
  //if the contact value is true, the door has been closed
  return !( pt.get<bool>("contact") );
}

// Notification_logic_controller::Motion_sensor_state
// Notification_logic_controller::check_motion_sensor_state(const mqtt::const_message_ptr &zigbee_message_ptr){
//   //solution to double notification problems
//   //we discard the second message with the same value of occupancy tag
//   //because sensor port always
//   //sends two messages for each event: closed door and opened door
//   static int previous_occupancy = -1;
//   std::string payload( zigbee_message_ptr->to_string() );
//   std::stringstream ss;
   
//   D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << "payload: '" << payload << '\n';)
//     ss << payload;
   
//   boost::property_tree::ptree pt;
//   boost::property_tree::read_json(ss, pt);
//   Motion_sensor_state motion_sensor_state{false, false};
//   motion_sensor_state.actual_occupancy = pt.get<bool>("occupancy");
//   //occupancy value is true or false, but we want to convert to an int value
//   int actual_occupancy = motion_sensor_state.actual_occupancy;

//   //  //I tried to put #undef DEBUG here to avoid printing but it doesn't work
//   // #if 0
//   //   D(std::cout << info << "Notification_logic_controller::" << "] " << reset <<
//   //     "actual contact: " << actual_contact << '\n';)
//   // #endif

  
//   //if actual_contact is false the door has been open and we have to check
//   //if it a dublicate because in this case no event will be trigger
//   if(actual_occupancy != previous_occupancy){
//     previous_occupancy = actual_occupancy;
//     motion_sensor_state.is_a_duplicate = false;
//   }
//   else
//     motion_sensor_state.is_a_duplicate = true;

//   return motion_sensor_state;
// }


Notification_logic_controller::Is_room_occupied
Notification_logic_controller::is_room_occupied(const mqtt::const_message_ptr &zigbee_message_ptr){
  std::string payload( zigbee_message_ptr->to_string() );
  std::stringstream ss;
   
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << "payload: '" << payload << '\n');
  ss << payload;
   
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);
  return pt.get<bool>("occupancy");
}

void Notification_logic_controller::update_area_protection(){
  _area_protection.update();
}

void Notification_logic_controller::analyze_ai_response(const mqtt::const_message_ptr &message_ptr){
  D(std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << "topic: " << message_ptr->get_topic() << '\n');
  std::string payload{message_ptr->get_payload()};
  D(std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << "payload: " << payload << std::endl);
  std::time_t now;
  std::time (&now);
  D( std::cout << error << "[Notification_logic_controller::" << __func__ << "]. " << reset
     << " time: " << std::ctime(&now) << std::endl);

  std::stringstream ss;
  ss << payload;
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);
  std::string position =  pt.get<Position>("position");
  Ai_result ai_result = static_cast<Ai_result>( pt.get<int>("response") );
  //triggering the event
  _ai_result_position_proc_events_map[ std::make_pair(ai_result, position) ]();

}

// mqtt::const_message_ptr Notification_logic_controller::prepare_rich_notification(const mqtt::const_message_ptr &zigbee_message_ptr, const std::string &sensor_mini_id){
//   static std::string last_sent_short_filename{};
//   boost::ignore_unused(zigbee_message_ptr);
//   boost::property_tree::ptree pt;
//   Dir_handler dir_handler{ _cam_path[ _sensor_cam[sensor_mini_id] ] };
//   //this snippet must be executed just once to check that the dir exists
//   //to avoid unnecessary execution, we use this trick that uses lambda function
//   static bool once = [&dir_handler]{
//     std::cout << "just once" << std::endl;
//     if( !dir_handler.exists() ){
//       std::cerr << "[Notification_logic_controller::" << __func__ << "]. " << "no directory with mp4 file. " << std::endl; 
//       exit(1);
//     }
//     return true;
//   }();
//   /////////////////////////////////////////////////////////////////////////////////////
//   boost::ignore_unused(once);
//   std::time_t now;
//   std::time (&now);
//   D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
//      << "The current local time is: " << std::ctime(&now) ); //ctime adds automatically a \n
//   Dir_handler::Time_path_pair to_send = dir_handler.get_last_modified_file(".mp4");
//   std::string to_send_filename = ( (to_send.second).filename() ).string();
//   if( !( to_send_filename.empty() ) ){
//     D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
//        << "Video " /*<< (iter + 1) <<*/" to publish is " << to_send_filename << std::endl );
//     //we take only the last 10 chars because the filename is very
//     //long and the comparison between the entire filename can be heavy
//     std::string curr_short_filename = to_send_filename.substr(to_send_filename.size() - 10);
//     D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
//        << "curr: " << curr_short_filename << " last: " << last_sent_short_filename << std::endl);
//     if(curr_short_filename != last_sent_short_filename){
//       last_sent_short_filename = curr_short_filename;
//       //let's wait until the video chunk to send is finished:
//       //the condition for this to happen is the presence of a next video chunk 
//       while(true){
// 	Dir_handler::Time_path_pair next = dir_handler.get_last_modified_file(".mp4");
// 	if( ( (next.second).filename() ).string() != to_send_filename )
// 	  break;
// 	std::this_thread::sleep_for ( std::chrono::milliseconds(200) ); 
//       }
  
//       pt.put("filename", to_send_filename);
//       //pt.put( "data", base64_file_converter( (to_send.second).string() ) );
//       cv::VideoCapture cap( (to_send.second).string() );
//       if ( !cap.isOpened() ){
// 	std::cerr << error << "[Notification_logic_controller::" << __func__ << "]. "
// 		  << reset << "Cannot open the video file" << std::endl;
// 	exit(1);
//       }
//       //let's go to 0.8 second in the video
//       cap.set(CV_CAP_PROP_POS_MSEC, 500);
//       cv::Mat frame;
//       bool b_success = cap.read(frame);
//       if ( !b_success) {
// 	std::cerr << error << "[Notification_logic_controller::" << __func__ << "]. "
// 		  << reset << "Cannot read the frame from video file" << std::endl;
// 	exit(1);
//       }

//       std::string jpeg_filename( (to_send.second).string() + ".jpeg" );
//       std::vector<int> jpeg_params;
//       jpeg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
//       jpeg_params.push_back(80);
      
//       imwrite(jpeg_filename, frame, jpeg_params);
//       pt.put( "data", base64_file_converter(jpeg_filename) );
      
//       std::stringstream ss;
//       boost::property_tree::json_parser::write_json(ss, pt);
//       //if we put assignment here, we have double sending of the same file
//       //last_sent_short_filename = curr_short_filename;
//       D( std::cout << error << "[Notification_logic_controller::" << __func__ << "]. " << reset
// 	 << "filename: " << to_send_filename << " time: " << std::ctime(&now) << std::endl);
//       return mqtt::make_message( _publisher.get_topic(), ss.str() );
//     }else{
//       D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
// 	 << "Video alredy sent" << std::endl );
//       return mqtt::make_message("", "");
//     }
//   }
//   else{
//     std::cerr << error << "[Notification_logic_controller::" << __func__ << "]. " << reset
// 	      <<"Video to publish is an empty string: sending a null message" << std::endl;
//     return mqtt::make_message("", "");
//   }
// }

void Notification_logic_controller::send_video_chunk(const Ext_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  send_rich_notifications(evt._sensor_mini_id, 1, 2);
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << std::endl);
}
void Notification_logic_controller::send_video_chunk(const Int_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << std::endl);
}

void Notification_logic_controller::send_video_chunk(const Res_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << std::endl);
}


void Notification_logic_controller::increase_ai_response_counter(const Ext_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  _number_of_sent_frames += _NUMBER_OF_FRAMES_TO_SEND;
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << std::endl);
}
void Notification_logic_controller::increase_ai_response_counter(const Int_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  _number_of_sent_frames += _NUMBER_OF_FRAMES_TO_SEND;
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << std::endl);
}

void Notification_logic_controller::increase_ai_response_counter(const Res_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  _number_of_sent_frames += _NUMBER_OF_FRAMES_TO_SEND;
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << std::endl);
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_owner_in_ext &evt){
  _number_of_received_responses += 1;
  if(_number_of_sent_frames == _number_of_received_responses)
    process_event_verbose(Ai_response_off{});
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_owner_in_int &evt){
  _number_of_received_responses += 1;
  if(_number_of_sent_frames == _number_of_received_responses)
    process_event_verbose(Ai_response_off{});  
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_owner_in_res &evt){
  _number_of_received_responses += 1;
  if(_number_of_sent_frames == _number_of_received_responses)
      process_event_verbose(Ai_response_off{});
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_monit_in_ext &evt){
  _number_of_received_responses += 1;
  if(_number_of_sent_frames == _number_of_received_responses)
    process_event_verbose(Ai_response_off{});
}
void Notification_logic_controller::decrease_ai_response_counter(const Rec_monit_in_int &evt){
  _number_of_received_responses += 1;
    if(_number_of_sent_frames == _number_of_received_responses)
      process_event_verbose(Ai_response_off{});
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_monit_in_res &evt){
  _number_of_received_responses += 1;
  if(_number_of_sent_frames == _number_of_received_responses)
    process_event_verbose(Ai_response_off{});
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_unk_in_ext &evt){
  _number_of_received_responses += 1;
  if(_number_of_sent_frames == _number_of_received_responses)
    process_event_verbose(Ai_response_off{});
  
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_unk_in_int &evt){
  _number_of_received_responses += 1;
  if(_number_of_sent_frames == _number_of_received_responses)
    process_event_verbose(Ai_response_off{});  
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_unk_in_res &evt){
  _number_of_received_responses += 1;
  if(_number_of_sent_frames == _number_of_received_responses)
    process_event_verbose(Ai_response_off{});  
}

void Notification_logic_controller::load_configuration(const std::string &configuration_file){
  //I dont't know why boost::bind doesn't work
  //part in which we'll parse the file to retrieve information for loading the map
  // _sensor_proc_events_map["01cc99b3"] = std::bind(
  // 						    &Alarm_system::process_event_verbose,
  // 						    this,
  // 						    Ext_door_open_sensor_sig{"cam02"}
  // 						    );

  //TODO
  //This maps can collpse into one in which the value is a struct with all infos
  _sensor_position_map["01cc99b3"] = "ext";
  _sensor_position_map["0202c411"] = "ext";
  _sensor_type_map["01cc99b3"] = Sensor_type::CONTACT;
  _sensor_type_map["0202c411"] = Sensor_type::MOTION;
  _sensor_proc_events_map["01cc99b3"] = [this](){ return process_event_verbose(Ext_door_open_sensor_sig{"01cc99b3"}); };
  _sensor_proc_events_map["doorfkeI"] = [this](){ return process_event_verbose(Int_door_open_sensor_sig{"12345678"}); };
  _sensor_proc_events_map["doorfkeR"] = [this](){ return process_event_verbose(Res_door_open_sensor_sig{"90123456"}); };
  _sensor_proc_events_map["0202c411"] = [this](){ return process_event_verbose(Ext_motion_sensor_sig{}); };
  _sensor_proc_events_map["motfke_I"] = [this](){ return process_event_verbose(Int_motion_sensor_sig{}); };
  _sensor_proc_events_map["motfke_R"] = [this](){ return process_event_verbose(Res_motion_sensor_sig{}); };

  
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::UNKNOWN, "ext")] = [this](){ return process_event_verbose(Rec_unk_in_ext{}); };
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::UNKNOWN, "int")] = [this](){ return process_event_verbose(Rec_unk_in_int{}); }; 
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::UNKNOWN, "res")] = [this](){ return process_event_verbose(Rec_unk_in_res{}); }; 					      

  _ai_result_position_proc_events_map[std::make_pair(Ai_result::OWNER, "ext")] = [this](){ return process_event_verbose(Rec_owner_in_ext{}); };
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::OWNER, "int")] = [this](){ return process_event_verbose(Rec_owner_in_int{}); }; 
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::OWNER, "res")] = [this](){ return process_event_verbose(Rec_owner_in_res{}); }; 					      

  _ai_result_position_proc_events_map[std::make_pair(Ai_result::MONITORED, "ext")] = [this](){ return process_event_verbose(Rec_monit_in_ext{}); };
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::MONITORED, "int")] = [this](){ return process_event_verbose(Rec_monit_in_int{}); }; 
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::MONITORED, "res")] = [this](){ return process_event_verbose(Rec_monit_in_res{}); }; 					      


  process_event_verbose(Initialization_completed{});
}



