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
#include <chrono>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>       
#include <boost/lexical_cast.hpp>

Notification_logic_controller::Notification_logic_controller(Area_protection &area_protection,
							     Synchronized_queue<mqtt::const_message_ptr> &queue,
							     Publisher &publisher,
							     //							     std::map<std::string, std::string> &sensor_cam,  
							     //							     std::map<std::string, std::string> &cam_path,
							     const int JPEG_QUALITY,
							     const int WHICH_FRAME,
							     const int NUMBER_OF_FRAMES_TO_SEND,
							     const int NUMBER_OF_AI_RESPONSES):
  _area_protection{area_protection},
  _queue{queue},
  _publisher{publisher},
  // _sensor_cam{},
  // _cam_path{},
  _sensor_cam_path{},
  _ai_response_counter{0},
  _WHICH_FRAME{WHICH_FRAME},
  _JPEG_QUALITY{JPEG_QUALITY},
  _NUMBER_OF_FRAMES_TO_SEND{NUMBER_OF_FRAMES_TO_SEND},
  _NUMBER_OF_AI_RESPONSES{NUMBER_OF_AI_RESPONSES},
  _SIGNIFICANT_TOPIC_CHARS{8},
  _is_ext_occupied{false},
  _is_int_occupied{false},
  _is_res_occupied{false}{}

void Notification_logic_controller::consume_message(){
  while(true){
    mqtt::const_message_ptr msg =  _queue.get();
    //the following is a blocking operation till these
    //sent of the messages
    //classify_message(msg);
    std::thread(&Notification_logic_controller::classify_message, this, msg).detach();
  }
}

void Notification_logic_controller::classify_message(const mqtt::const_message_ptr &zigbee_message_ptr){
  //topic is in the form of zigbee2mqtt/0x00158d0001cc99b3
  std::string topic = zigbee_message_ptr->get_topic();
  //and we want to extract only the last 8 digits guessing that are
  //sufficient to uniquely distinguish a sensor
  std::string topic_info = topic.substr(topic.size() - _SIGNIFICANT_TOPIC_CHARS);
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "] " << reset
    << "topic_info is: " << topic_info << '\n');
  //let's do a search in the _sensor_cam map to decide if we have
  //to prepare a rich notification or a classified notification
  mqtt::const_message_ptr message_to_send;

  //auto it = _sensor_proc_events_map.find(topic_info);
  auto it = _sensor_infos_map.find(topic_info);
  //if( it != _sensor_proc_events_map.end() ){
  if( it != _sensor_infos_map.end() ){
    //switch(_sensor_type_map[topic_info]){
    //switch(_sensor_infos_map[topic_info]._sensor_type){
    switch( (it->second)._sensor_type){
    case Sensor_type::DOOR :
    case Sensor_type::WINDOW :  
      if( is_gate_opened(zigbee_message_ptr) ){
	D( Time_spent<> to_send_notifications );
	//( (it->second).first )();
	( ( (it->second)._proc_events_ptr_pair ).first )();
      }
      else
	return;
      break;
    case Sensor_type::MOTION : {
      D( Time_spent<> to_send_notifications );
      if( check_motion_sensor_state(zigbee_message_ptr) ){
	//triggering Ext_motion_sensor_sig/Int_motion_sensor_sig/Res_motion_sensor_sig
	// ( (it->second).first )();
	update_motion_sensor_state(topic_info, true);
	( ( (it->second)._proc_events_ptr_pair ).first )();
      }
      else{
	update_motion_sensor_state(topic_info, false);
	//triggering Clear_ext/Clear_int/Clear_res signals
	//( (it->second).second )();
	if( !is_ring_occupied(topic_info) ){
	  ( ( (it->second)._proc_events_ptr_pair ).second )();
	}
      }
      break;
    }
    default:
      std::cerr << error << " [Notification_logic_controller::" << __func__ << "] " << reset
    		<< "unrecognized sensor id" << std::endl;
    }//END OF SWITCH
  }
  // //if it's not in the sensor_proc_events_map, it must be
  //if it's not in the sensor_infos_map, it must be 
  //an answer from the ai server (in this case the topic is Response
  else if (topic_info == "ai_reply"){
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
    //we take only the last 14 chars because the filename is very
    //long and the comparison between the entire filename can be heavy
    std::string curr_short_filename = to_send_filename.substr(to_send_filename.size() - 14);
    D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
       << "curr: " << curr_short_filename << " last: " << last_sent_short_filename << std::endl);
    // if(curr_short_filename != last_sent_short_filename){
    //   last_sent_short_filename = curr_short_filename;
    //   mqtt::const_message_ptr message_to_send = prepare_rich_notification(to_send_ptr, file_type, sensor_mini_id);
    //   if(message_to_send->get_topic() != ""){
    // 	D(std::cout << info << "[Notification_logic_controller::" << __func__ << "] " << reset
    //   	  << "message_to_send size is: " << ( message_to_send->to_string() ).size() << '\n';)
    // 	  // //print on standard error only for integration test of 08/08/2019
    // 	  // std::cerr << "message" << std::endl;
    // 	  // std::cerr << message_to_send->get_payload() << std::endl;
    // 	  send_notification(message_to_send);
    //   }
    // }
    // else{
    //   D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
    // 	 << "Video already sent" << std::endl );
    // }
    int size{0}, i{0};
    mqtt::const_message_ptr message_to_send{};
    do{
      message_to_send = prepare_rich_notification(to_send_ptr, file_type, sensor_mini_id, _JPEG_QUALITY - 5*i);
      ++i;
      size = ( message_to_send->to_string() ).size(); 
    }while( (size > 127000) && (i < 3) );
    if(message_to_send->get_topic() != ""){
      if(curr_short_filename == last_sent_short_filename)
	D( std::cout << info << "[Notification_logic_controller::" << __func__ << "]. " << reset
	   << "Video already sent" << std::endl );
      else
	last_sent_short_filename = curr_short_filename;
      D(std::cout << info << "[Notification_logic_controller::" << __func__ << "] " << reset
	<< "message_to_send size is: " << size << '\n';)
	// //print on standard error only for integration test of 03/10/2019
	// std::cerr << "message" << std::endl;
	// std::cerr << message_to_send->get_payload() << std::endl;
	send_notification(message_to_send);
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

mqtt::const_message_ptr Notification_logic_controller::prepare_rich_notification(const std::unique_ptr<Dir_handler::Time_path_pair> &to_send_ptr, File_type file_type, const std::string &sensor_mini_id, int jpeg_quality){
  using namespace std::chrono;
  boost::property_tree::ptree pt;
  pt.put("ts", duration_cast<milliseconds>(
					   system_clock::now().time_since_epoch()
					   ).count()
	 );
  pt.put("type",  "iq");
  //pt.put("hub", _hub_id);
  //pt.put("ring", _sensor_position_map[sensor_mini_id]);
  pt.put("ring", _sensor_infos_map[sensor_mini_id]._position );
  pt.put("service", "fv");
  pt.put("muuid", boost::lexical_cast<std::string>( boost::uuids::random_generator()() ) );
  //TODO: understand if the to_send_filename is necessary anymore
  std::string to_send_filename = ( (to_send_ptr->second).filename() ).string();  
  if(file_type == MP4){
    //pt.put("filename", to_send_filename );
    pt.put( "media", base64_file_converter( (to_send_ptr->second).string() ) );
  }
  else if(file_type == JPEG){
    static std::string last_jpeg_file{};
    //let's remove last jpeg file
    std::remove( last_jpeg_file.c_str() );
    //pt.put("filename", to_send_filename.replace(to_send_filename.find_last_of('.'), std::string::npos, ".jpeg") );
    //TODO: understand if the follwing line is necessary
    //to_send_filename.replace(to_send_filename.find_last_of('.'), std::string::npos, ".jpeg");
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

    // if ( !b_success) {
    //   std::cerr << error << "[Notification_logic_controller::" << __func__ << "]. "
    // 		<< reset << "Cannot read the frame from video file" << std::endl;
    //   exit(1);
    // }
    // std::string mp4_complete_filename = (to_send_ptr->second).string();
    // std::string jpeg_complete_filename( mp4_complete_filename.replace(mp4_complete_filename.find_last_of('.'), std::string::npos, ".jpeg") );
      
    // imwrite(jpeg_complete_filename, frame, _jpeg_params);
    
    // pt.put( "media", base64_file_converter(jpeg_complete_filename) );
    // last_jpeg_file = jpeg_complete_filename;

    //
    if(b_success){
      std::string mp4_complete_filename = (to_send_ptr->second).string();
      std::string jpeg_complete_filename( mp4_complete_filename.replace(mp4_complete_filename.find_last_of('.'), std::string::npos, ".jpeg") );

      std::vector<int> jpeg_params;
      jpeg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
      
      std::size_t found = mp4_complete_filename.find("foscam");
      //for foscam cam jpeg quality must be set to a lower level to have payload of 128KB
      if (found != std::string::npos){
	jpeg_quality -= 10;
	jpeg_params.push_back(jpeg_quality);
      }
      else
	jpeg_params.push_back(jpeg_quality);
      imwrite(jpeg_complete_filename, frame, jpeg_params);
    
      pt.put( "media", base64_file_converter(jpeg_complete_filename) );
      last_jpeg_file = jpeg_complete_filename;
    }
    else{
      std::cerr << error << "[Notification_logic_controller::" << __func__ << "]. "
		<< reset << "Cannot read the frame from video file" << std::endl;
      //as Ai waits an exact number of files, in case of error we send a fake image with nobody
      to_send_filename = "fake_image.jpg";
      pt.put( "media", base64_file_converter("/home/pi/gstreamer_projects/multifiles_saving/fake_image.jpg") );
    }
    //
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
     << "filename: " << to_send_filename /*<< " time: " << std::ctime(&now)*/ << " with quality: " << jpeg_quality << std::endl);
  return mqtt::make_message( _publisher.get_topic(), ss.str() );
}

std::unique_ptr<Dir_handler::Time_path_pair>
Notification_logic_controller::select_video_chunk(const std::string &sensor_mini_id, int which){
  //Dir_handler dir_handler{ _cam_path[ _sensor_cam[sensor_mini_id] ] };
  Dir_handler dir_handler{ _sensor_cam_path[sensor_mini_id] };
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


Notification_logic_controller::Motion_sensor_state
Notification_logic_controller::check_motion_sensor_state(const mqtt::const_message_ptr &zigbee_message_ptr){
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
  // //print in standard error only for integration test of 03/10/2019
  // std::cerr << "message" << std::endl;
  // std::cerr <<  payload << std::endl;
  D(std::cout << warning << "[Notification_logic_controller::" << __func__ << "]. " << reset << "payload: " << payload << std::endl);
  std::time_t now;
  std::time (&now);
  D( std::cout << error << "[Notification_logic_controller::" << __func__ << "]. " << reset
     << " time: " << std::ctime(&now) << std::endl);

  std::cout << 0 << std::endl;
  std::stringstream ss;
  ss << payload;
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);
  char position =  pt.get<Position>("ring");
  //Ai_result ai_result = static_cast<Ai_result>( pt.get<int>("response") );
  int number_of_owners = pt.get<int>("owner");
  int number_of_monitored = pt.get<int>("monitored");
  int number_of_unknown = pt.get<int>("unknown");
  std::string mmuid = pt.get<std::string>("muuid");
  //triggering the event
  Ai_result ai_result = decode_ai_result(number_of_owners, number_of_monitored, number_of_unknown);
  if(ai_result != Ai_result::NO_DETECTION)
    _ai_result_position_proc_events_map[ std::make_pair(
							ai_result,
							position)
					 ](mmuid);

}

Notification_logic_controller::Ai_result Notification_logic_controller::decode_ai_result(int number_of_owners,
											 int number_of_monitored,
											 int number_of_unknown){
  if( !number_of_owners && !number_of_monitored && !number_of_unknown)
    return Ai_result::NO_DETECTION;
  else if(number_of_owners)
    return Ai_result::OWNER;
  else if(number_of_monitored)
    return Ai_result::MONITORED;
  else if(number_of_unknown)
    return Ai_result::UNKNOWN;
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
// 	 << "Video already sent" << std::endl );
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
  send_rich_notifications(evt._sensor_mini_id, _WHICH_FRAME, _NUMBER_OF_FRAMES_TO_SEND - 1);
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << std::endl);
}
void Notification_logic_controller::send_video_chunk(const Int_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  send_rich_notifications(evt._sensor_mini_id, _WHICH_FRAME, _NUMBER_OF_FRAMES_TO_SEND - 1);
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << std::endl);
}

void Notification_logic_controller::send_video_chunk(const Res_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  send_rich_notifications(evt._sensor_mini_id, _WHICH_FRAME, _NUMBER_OF_FRAMES_TO_SEND - 1);
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << std::endl);
}

void Notification_logic_controller::send_classified_notification_av(char alarm_level, const std::string &mmuid){
  using namespace std::chrono;
  boost::property_tree::ptree pt;
  pt.put("ts", duration_cast<milliseconds>(
					   system_clock::now().time_since_epoch()
					   ).count()
	 );
  pt.put("type",  "av");
  //pt.put("hub", _hub_id);
  pt.put("level", alarm_level);
  pt.put("srcid", mmuid);
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, pt);
  // //print in standard error only for integration test of 03/10/2019
  // std::cerr << "message" << std::endl;
  // std::cerr << ss.str() << std::endl;
  _publisher.publish( mqtt::make_message("alarm", ss.str() ) );
}

void Notification_logic_controller::send_classified_notification_as(char alarm_level, const std::string &sensor_mini_id){
  using namespace std::chrono;
  boost::property_tree::ptree pt;
  pt.put("ts", duration_cast<milliseconds>(
					   system_clock::now().time_since_epoch()
					   ).count()
	 );
  pt.put("type",  "as");
  //pt.put("hub", _hub_id);
  pt.put("level", alarm_level);
  pt.put("srcid", sensor_mini_id);
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, pt);
  // //print in standard error only for integration test of 03/10/2019
  // std::cerr << "message" << std::endl;
  // std::cerr << ss.str() << std::endl;
  _publisher.publish( mqtt::make_message("alarm", ss.str() ) );
}

void Notification_logic_controller::increase_ai_response_counter(){
  _ai_response_counter += _NUMBER_OF_AI_RESPONSES;
  D(std::cout << info << "[Notification_logic_controller::" << __func__ << "]. "  << reset << std::endl);
}

void Notification_logic_controller::increase_ai_response_counter(const Ext_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  increase_ai_response_counter();
}
void Notification_logic_controller::increase_ai_response_counter(const Int_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  increase_ai_response_counter();
}

void Notification_logic_controller::increase_ai_response_counter(const Res_door_open_sensor_sig &evt){
  boost::ignore_unused(evt);
  increase_ai_response_counter();
}

void Notification_logic_controller::decrease_ai_response_counter(){
  //to avoid problems of multiple sending of the same response from ai_server (due to qos = 1)
  //let's decrease _ai_response_counter only when this number is greater than 0
  if(_ai_response_counter > 0){
    _ai_response_counter -= 1;
    //if after decrement _ai_response_counters is 0 we received all the expected replies
    if(!_ai_response_counter)
      process_event_verbose(Ai_response_off{});
  }
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_owner_in_ext &){
  decrease_ai_response_counter();
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_owner_in_int &){
  decrease_ai_response_counter();
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_owner_in_res &){
  decrease_ai_response_counter();
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_monit_in_ext &){
  decrease_ai_response_counter();
}
void Notification_logic_controller::decrease_ai_response_counter(const Rec_monit_in_int &){
  decrease_ai_response_counter();
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_monit_in_res &){
  decrease_ai_response_counter();
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_unk_in_ext &){
  decrease_ai_response_counter();  
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_unk_in_int &){
  decrease_ai_response_counter();
}

void Notification_logic_controller::decrease_ai_response_counter(const Rec_unk_in_res &){
  decrease_ai_response_counter();
}

void Notification_logic_controller::ext_presence_flag_update(const Rec_owner_in_ext &){
  _is_ext_occupied = true;
}
void Notification_logic_controller::ext_presence_flag_update(const Rec_monit_in_ext &){
  _is_ext_occupied = true;
}
void Notification_logic_controller::ext_presence_flag_update(const Rec_unk_in_ext &){
  _is_ext_occupied = true;
}
void Notification_logic_controller::int_presence_flag_update(const Rec_owner_in_int &){
  _is_int_occupied = true;
}
void Notification_logic_controller::int_presence_flag_update(const Rec_monit_in_int &){
  _is_int_occupied = true;
}
void Notification_logic_controller::int_presence_flag_update(const Rec_unk_in_int &){
  _is_int_occupied = true;
}
void Notification_logic_controller::res_presence_flag_update(const Rec_owner_in_res &){
  _is_res_occupied = true;
}
void Notification_logic_controller::res_presence_flag_update(const Rec_monit_in_res &){
  _is_res_occupied = true;
}
void Notification_logic_controller::res_presence_flag_update(const Rec_unk_in_res &){
  _is_res_occupied = true;
}

void Notification_logic_controller::ext_presence_flag_reset(const Clear_ext &){
  _is_ext_occupied = false;
}
void Notification_logic_controller::int_presence_flag_reset(const Clear_int &){
  _is_int_occupied = false;
}
void Notification_logic_controller::res_presence_flag_reset(const Clear_res &){
  _is_res_occupied = false;
}

void Notification_logic_controller::load_configuration(const std::string &configuration_file_path){
  //TODO
  //cam01, cam02 and cam03 must be replaced by most explicit names:
  //external for cam01: /home/pi/gstreamer/multifiles_saving/external
  //internal for cam03: /home/pi/gstreamer/multifiles_saving/internal
  //reserverd for cam02: /home/pi/gstreamer/multifiles_saving/reserved
  //this is wrong, beacuse we can have multiple cams in the same gate
  // _sensor_cam_path["01cc9efa"] = "/home/pi/gstreamer/multifiles_saving/cam01";
  // _sensor_cam_path["01ccf6bb"] = "/home/pi/gstreamer/multifiles_saving/cam03";
  // _sensor_cam_path["01cc99b3"] = "/home/pi/gstreamer/multifiles_saving/cam02";

  boost::property_tree::ptree pt;
  try{
    boost::property_tree::read_json(configuration_file_path, pt);
  }catch(std::exception &e){
    std::cerr << error << "[Notification_logic_controller::" << __func__ << reset << "Error: " << e.what() << std::endl;
  }

  //setting the number of state machine's levels
  set_number_of_levels( parse_number_of_levels(pt) );
  //setting the paths where cam we'll save videos 
  init_sensor_cam_path(pt);

  //setting the map with the statemachine action trigger
  Sensor_mini_ids sensor_mini_ids;
  associate_sensor_to_events(pt,
  			     "external",
  			     Sensor_type::DOOR,
  			     sensor_mini_ids);
  for(auto item : sensor_mini_ids)
    _sensor_infos_map[item] = { Sensor_type::DOOR, 'e', std::make_pair( [this, item](){ return process_event_verbose(Ext_door_open_sensor_sig{item}); }, [this](){ return; } ) };

  sensor_mini_ids.clear();
  associate_sensor_to_events(pt,
  			     "external",
  			     Sensor_type::MOTION,
  			     sensor_mini_ids);
  for(auto item : sensor_mini_ids){
    _sensor_infos_map[item] = { Sensor_type::MOTION, 'e', std::make_pair( [this, item](){ return process_event_verbose(Ext_motion_sensor_sig{item}); }, [this](){ return process_event_verbose(Clear_ext{}); } ) };
    _extern_motion_sensors_state_map[item] = false;
    _motion_sensors_maps[item] = &_extern_motion_sensors_state_map; 
  }

  sensor_mini_ids.clear();
  associate_sensor_to_events(pt,
  			     "internal",
  			     Sensor_type::DOOR,
  			     sensor_mini_ids);
  for(auto item : sensor_mini_ids)
    _sensor_infos_map[item] = { Sensor_type::DOOR, 'i',  std::make_pair( [this, item](){ return process_event_verbose(Int_door_open_sensor_sig{item}); }, [this](){ return; } )} ;

  sensor_mini_ids.clear();
  associate_sensor_to_events(pt,
  			     "internal",
  			     Sensor_type::MOTION,
  			     sensor_mini_ids);
  for(auto item : sensor_mini_ids){
    _sensor_infos_map[item] = { Sensor_type::MOTION, 'i', std::make_pair( [this, item](){ return process_event_verbose(Int_motion_sensor_sig{item}); }, [this](){ return process_event_verbose(Clear_int{}); } ) };
    _intern_motion_sensors_state_map[item] = false;
    _motion_sensors_maps[item] = &_intern_motion_sensors_state_map;
  }

  sensor_mini_ids.clear();
  associate_sensor_to_events(pt,
  			     "internal",
  			     Sensor_type::WINDOW,
  			     sensor_mini_ids);
  for(auto item : sensor_mini_ids)
    _sensor_infos_map[item] = { Sensor_type::WINDOW, 'i', std::make_pair( [this, item](){ return process_event_verbose(Int_wind_open_sensor_sig{item}); }, [this](){ return; } ) };

  sensor_mini_ids.clear();
  associate_sensor_to_events(pt,
  			     "reserved",
  			     Sensor_type::DOOR,
  			     sensor_mini_ids);
  for(auto item : sensor_mini_ids)
    _sensor_infos_map[item] = { Sensor_type::DOOR, 'r', std::make_pair( [this, item](){ return process_event_verbose(Res_door_open_sensor_sig{item}); }, [this](){ return; } ) };

  sensor_mini_ids.clear();
  associate_sensor_to_events(pt,
  			     "reserved",
  			     Sensor_type::MOTION,
  			     sensor_mini_ids);
  for(auto item : sensor_mini_ids){
    _sensor_infos_map[item] = { Sensor_type::MOTION, 'r', std::make_pair( [this, item](){ return process_event_verbose(Res_motion_sensor_sig{item}); }, [this](){ return process_event_verbose(Clear_res{}); } ) };
    _reserved_motion_sensors_state_map[item] = false;
    _motion_sensors_maps[item] = &_reserved_motion_sensors_state_map;
  }

  sensor_mini_ids.clear();
  associate_sensor_to_events(pt,
  			     "reserved",
  			     Sensor_type::WINDOW,
  			     sensor_mini_ids);
  for(auto item : sensor_mini_ids)
    _sensor_infos_map[item] = { Sensor_type::WINDOW, 'r', std::make_pair( [this, item](){ return process_event_verbose(Res_wind_open_sensor_sig{item}); }, [this](){ return; } ) };
  ////////////////////////////////////////////////////////////////////////////////////////
  
  // //just for testing in test_configuration_file.cc
  // _sensor_infos_map_2["01cc9efa"] = { Sensor_type::DOOR, 'e', std::make_pair( [this](){ return process_event_verbose(Ext_door_open_sensor_sig{"01cc9efa"}); }, [this](){ return; } ) };
  // _sensor_infos_map_2["0202c411"] = { Sensor_type::MOTION, 'e', std::make_pair( [this](){ return process_event_verbose(Ext_motion_sensor_sig{"0202c411"}); }, [this](){ return process_event_verbose(Clear_ext{}); } ) };
  // _sensor_infos_map_2["01ccf6bb"] = { Sensor_type::DOOR, 'i',  std::make_pair( [this](){ return process_event_verbose(Int_door_open_sensor_sig{"01ccf6bb"}); }, [this](){ return; } )} ;
  // _sensor_infos_map_2["0202c38b"] = { Sensor_type::MOTION, 'i', std::make_pair( [this](){ return process_event_verbose(Int_motion_sensor_sig{"0202c38b"}); }, [this](){ return process_event_verbose(Clear_int{}); } ) };
  // _sensor_infos_map_2["01ccfa8f"] = { Sensor_type::WINDOW, 'i', std::make_pair( [this](){ return process_event_verbose(Int_wind_open_sensor_sig{"01ccfa8f"}); }, [this](){ return; } ) };
  // _sensor_infos_map_2["01cc99b3"] = { Sensor_type::DOOR, 'r', std::make_pair( [this](){ return process_event_verbose(Res_door_open_sensor_sig{"01cc99b3"}); }, [this](){ return; } ) };
  // _sensor_infos_map_2["motion_r"] = { Sensor_type::MOTION, 'r', std::make_pair( [this](){ return process_event_verbose(Res_motion_sensor_sig{"motion_r"}); }, [this](){ return process_event_verbose(Clear_res{}); } ) };
  // _sensor_infos_map_2["window_r"] = { Sensor_type::WINDOW, 'r', std::make_pair( [this](){ return process_event_verbose(Res_wind_open_sensor_sig{"window_r"}); }, [this](){ return; } ) };

  _ai_result_position_proc_events_map[std::make_pair(Ai_result::UNKNOWN, 'e')] = [this](const std::string &mmuid){ return process_event_verbose(Rec_unk_in_ext{mmuid}); };
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::UNKNOWN, 'i')] = [this](const std::string &mmuid){ return process_event_verbose(Rec_unk_in_int{mmuid}); }; 
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::UNKNOWN, 'r')] = [this](const std::string &mmuid){ return process_event_verbose(Rec_unk_in_res{mmuid}); }; 
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::OWNER, 'e')] = [this](const std::string &mmuid){ return process_event_verbose(Rec_owner_in_ext{mmuid}); };
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::OWNER, 'i')] = [this](const std::string &mmuid){ return process_event_verbose(Rec_owner_in_int{mmuid}); }; 
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::OWNER, 'r')] = [this](const std::string &mmuid){ return process_event_verbose(Rec_owner_in_res{mmuid}); }; 
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::MONITORED, 'e')] = [this](const std::string &mmuid){ return process_event_verbose(Rec_monit_in_ext{mmuid}); };
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::MONITORED, 'i')] = [this](const std::string &mmuid){ return process_event_verbose(Rec_monit_in_int{mmuid}); }; 
  _ai_result_position_proc_events_map[std::make_pair(Ai_result::MONITORED, 'r')] = [this](const std::string &mmuid){ return process_event_verbose(Rec_monit_in_res{mmuid}); };

  _hub_id = "hub_di_tizio_caio_sempronio";


  for(auto item : _sensor_cam_path)
    std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " sensor: " << item.first << "   cam: " << item.second << '\n'; 
  
  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " printing sensor_infos_map" << '\n'; 
  for(auto item : _sensor_infos_map)
    std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " sensor id: " << item.first << " -> sensor type: " << static_cast<int>( (item.second)._sensor_type ) << " sensor position: " << (item.second)._position << '\n'; 

  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " number of levels: " << get_number_of_levels() << '\n'; 

  std::cout << warning << "[Notification_logic_controller::" << __func__ << reset << " printing _motion_sensors_maps" << '\n'; 
  for(auto outer_value : _motion_sensors_maps){
    std::cout << outer_value.first << " -> " << outer_value.second << '\n';
    for( auto inner_value : *(outer_value.second) )
      std::cout << '\t' << inner_value.first << " -> " << std::boolalpha << inner_value.second << '\n';
  }

  
  process_event_verbose(Initialization_completed{});
}


void Notification_logic_controller::init_sensor_cam_path(const boost::property_tree::ptree &pt){
  typedef std::pair<Sensor_mini_id, Cam_directory> Sensor_cam_pair;
  Sensor_cam_pair sensor_cam_pair;
  std::string base_path{"/home/pi/gstreamer_projects/multifiles_saving/"};
  try{
    //int i = 0;
    //let's retrieve the ptree under the ring path
    //each ptree is an iterable object: iterating over it returns a pair (boost::property_tree::ptree::value_type)
    //in wich the first member is the name of the child and the second is the child tree
    //in this case the ptrees under ring are external, internal and reserved
    for( const auto &ring_child : pt.get_child("ring") ){
      //std::cout << "Ring child number " << ++i << ": " << ring_child.first << std::endl;
      //ring_child.second are the trees under external, internal and reserved
      //they represent rooms, even if they don't have names in the configuration
      for(const auto &ext_int_res_child : ring_child.second) {
	//std::cout << "\tRoom name: " << (ext_int_res_child.second).get<std::string>("name") << '\n' ;
	//we are looking for gates in the room
	//not all rooms may have gates: for this reason we must test its presence
	boost::property_tree::ptree::const_assoc_iterator it = (ext_int_res_child.second).find("gate");
	if( it != ext_int_res_child.second.not_found() ){
	  for(const auto &gate_child : (ext_int_res_child.second).get_child("gate")) {
	    //std::cout << "\t\tGate name: " << (gate_child.second).get<std::string>("name") << '\n' ;
	    auto gate_devices_node = (gate_child.second).get_child("devices");
	    for(const auto &gate_devices_child : gate_devices_node) {
	      //std::cout << "\t\t\tGate devices id: " << (gate_devices_child.second).get<std::string>("id_secret_device") << '\n' ;
	      //if sensor_type == 3 is a cam, otherwise is the sensor that triggers the cam
	      if( static_cast<Sensor_type>( (gate_devices_child.second).get<int>("sensor_type") )
		  ==
		  Sensor_type::CAM
		  )
		sensor_cam_pair.second = base_path + (gate_devices_child.second).get<std::string>("id_secret_device");
	      else
		sensor_cam_pair.first = (gate_devices_child.second).get<std::string>("id_secret_device");		  
	    }
	  }
	  _sensor_cam_path.insert(sensor_cam_pair);
	}
	else{
	  std::cerr << error << "[Notification_logic_controller::" << __func__ << reset  << "\t\tGate is missing" << std::endl;
	}
      }
    }//END_OF BOOST_FOR_EACH
  }//END OF INNER TRY
  catch(std::exception &e){
    std::cerr << error << "[Notification_logic_controller::" << __func__ << reset << "Error: " << e.what() << std::endl;
  }
}

void Notification_logic_controller::associate_sensor_to_events(const boost::property_tree::ptree &pt,
							       const std::string &ring_type,
							       Sensor_type sensor_type,
							       Sensor_mini_ids &sensor_mini_ids){
  std::string complete_path{std::string{"ring."} + ring_type};
  for(const auto &ext_int_res_child : pt.get_child(complete_path)) {
    //std::cout << "\tRoom name: " << (ext_int_res_child.second).get<std::string>("name") << '\n' ;
    //we are looking for gates in the room
    //not all rooms may have gates: for this reason we must test its presence
    boost::property_tree::ptree::const_assoc_iterator it = (ext_int_res_child.second).find("gate");
    if( it != ext_int_res_child.second.not_found() ){
      //std::cout << "\t\tGate exists\n" ;
      for( const auto &gate_child : (ext_int_res_child.second).get_child("gate") ) {
	//std::cout << "\t\tGate name: " << (gate_child.second).get<std::string>("name") << '\n' ;
	auto gate_devices_node = (gate_child.second).get_child("devices");
	for(const auto &gate_devices_child : gate_devices_node) {
	  if( static_cast<Sensor_type>( (gate_devices_child.second).get<int>("sensor_type") ) == sensor_type )
	    sensor_mini_ids.push_back( (gate_devices_child.second).get<std::string>("id_secret_device") ); 	    
	}
      }
    }
    else{
      std::cerr << error << "[Notification_logic_controller::" << __func__ << reset << "\t\tGate is missing" << std::endl;
    }	
    for(const auto &devices_child : (ext_int_res_child.second).get_child("devices") ){
      // sensor_type_map.insert(std::make_pair((devices_child.second).get<std::string>("id_secret_device"),
      // 					    static_cast<Sensor_type>( (devices_child.second).get<int>("sensor_type") )				    )
      // 			     );
      if(static_cast<Sensor_type>( (devices_child.second).get<int>("sensor_type") ) ==
	 sensor_type)
	sensor_mini_ids.push_back( (devices_child.second).get<std::string>("id_secret_device") ); 
    }
  }
}

int Notification_logic_controller::parse_number_of_levels(const boost::property_tree::ptree &pt) const{
  std::string path_to_external{ std::string{"ring.external"} };
  auto external_child = pt.get_child(path_to_external);
  //let's retrieve the iterator to the external node to see if there are subnodes
  auto it = external_child.begin();
  if( it != external_child.end() )
    return HOUSE_WITH_GARDEN_N_OF_LEVELS;
  else
    return HOUSE_WITHOUT_GARDEN_N_OF_LEVELS;
}

void Notification_logic_controller::update_motion_sensor_state(const Sensor_mini_id &topic_info,
							       Motion_sensor_state motion_sensor_state){
  Motion_sensors_maps::iterator motion_sensors_maps_it = _motion_sensors_maps.find(topic_info);
  if( motion_sensors_maps_it != _motion_sensors_maps.end() ){
    Motion_sensors_state_map *ptr = motion_sensors_maps_it->second;
    Motion_sensors_state_map::iterator motion_sensors_state_map_it = ptr->find(topic_info);
    if( motion_sensors_state_map_it != ptr->end() )
      motion_sensors_state_map_it->second = motion_sensor_state;
    else
      std::cerr << error << " [Notification_logic_controller::" << __func__ << "] " << reset
		<< "no occurrence in  Motion_sensors_state_map associated to the id: " << topic_info << std::endl;
  }else
    std::cerr << error << " [Notification_logic_controller::" << __func__ << "] " << reset
	      << "no occurrence in motions_sensors_map associated to the id: " << topic_info << std::endl;
}

Notification_logic_controller::Is_ring_occupied Notification_logic_controller::is_ring_occupied(const Sensor_mini_id &topic_info){
  Motion_sensors_maps::iterator motion_sensors_maps_it = _motion_sensors_maps.find(topic_info);
  Is_ring_occupied is_ring_occupied{false};
  if( motion_sensors_maps_it != _motion_sensors_maps.end() ){
    Motion_sensors_state_map *ptr = motion_sensors_maps_it->second;
    for(const Motion_sensors_state_map::value_type &map_value : *ptr){
      is_ring_occupied = (is_ring_occupied || map_value.second);
      if(is_ring_occupied)
	return is_ring_occupied;
    }
  }
  else
    std::cerr << error << " [Notification_logic_controller::" << __func__ << "] " << reset
	      << "no occurrence in motions_sensors_map associated to the id: " << topic_info << std::endl;
  return is_ring_occupied;
}
