#include "Publisher.hh"
#include <iostream>
#include <thread>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <base64.hh> //for base64_file_converter

Publisher::Publisher(mqtt::async_client &client,
		     const std::string &topic,
		     Delivery_action_listener &listener,
		     Synchronized_queue<mqtt::const_message_ptr> &queue,
		     Dir_handler &dir_handler):
  _client{client},
  _topic(topic),
  _listener{listener},
  _queue(queue),
  _dir_handler{dir_handler}{}

// void Publisher::run(){
//   Time_spent<> time;
//   while(true){
//     mqtt::const_message_ptr msg = _queue.get();
//     int iter = 0, n_of_sending = 2;
//     while( iter < n_of_sending){
//       boost::property_tree::ptree pt;
//       if( !_dir_handler.exists() ){
// 	std::cerr << "[Publisher::" << __func__ << "]. " << "no directory with mp4 file. " << std::endl; 
// 	exit(-1);
//       }
//       Dir_handler::Time_path_pair to_send = _dir_handler.get_last_modified_file(".mp4");
//       std::string to_send_filename = ( (to_send.second).filename() ).string();
//       std::time_t now;
//       std::time (&now);
//       std::cout << "The current local time is: " << std::ctime(&now)  << '\n';
//       std::cout << "[Publisher::" << __func__ << "]. " << "video " << (iter + 1) << " to publish is " << to_send_filename << '\n';
//       //let's wait until the video chunk to send is finished:
//       //the condition for this to happen is the presence of a next video chunk 
//       while(true){
// 	Dir_handler::Time_path_pair next = _dir_handler.get_last_modified_file(".mp4");
// 	if( ( (next.second).filename() ).string() != to_send_filename )
// 	  break;
// 	std::this_thread::sleep_for ( std::chrono::milliseconds(500) ); 
//       }
    
//       pt.put("filename", to_send_filename);
//       pt.put( "data", base64_file_converter( (to_send.second).string() ) );
    
//       std::stringstream ss;
//       boost::property_tree::json_parser::write_json(ss, pt);
//       mqtt::message_ptr pubmsg = mqtt::make_message( _topic, ss.str() );
    
//       //no json
//       //mqtt::message_ptr pubmsg = mqtt::make_message( _topic, base64_file_converter("/home/pi/mqtt/mqtt/temp_saved_file_dir/2019-04-30_14:46:22_cam02_5.mp4") );
//       ////////////////////////////////////////////////////////////////
//       std::cout << "[Publisher::" << __func__ << "]. " << "publishing ..." << '\n';
//       try{
// 	_client.publish(pubmsg, nullptr, _listener);
// 	while( !_listener.is_done() ) 
// 	  std::this_thread::sleep_for(std::chrono::milliseconds(100));
//       }
//       catch(const mqtt::exception& exc) {
// 	std::cerr << exc.what() << std::endl;
// 	return ;
//       }
//       ++iter;
//     } //END OF WHILE LOOP FOR N_OF_SENDING
//   }//END OF WHILE(TRUE)
// }

void Publisher::publish(const mqtt::const_message_ptr &to_publish){
  try{
    _client.publish(to_publish, nullptr, _listener);
    while( !_listener.is_done() ) 
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  catch(const mqtt::exception& exc) {
    std::cerr << "[Publisher::"  << __func__ << "]" 
	      << exc.what() << std::endl;
    return ;
  }
}

Delivery_action_listener& Publisher::get_listener(){
  return _listener;
}

const std::string& Publisher::get_topic() const{
  return _topic;
}
