#include "Publisher.hh"
#include <iostream>
#include <thread>
#include <base64.hh> //for base64_file_converter
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Utils.hh"

Publisher::Publisher(mqtt::async_client &client,
		     const std::string &topic,
		     Delivery_action_listener &listener,
		     Synchronized_queue<mqtt::const_message_ptr> &queue):
  _client{client},
  _topic(topic),
  _listener{listener},
  _queue(queue){}

void Publisher::run(){
  Time_spent<> time;
  while(true){
    mqtt::const_message_ptr msg = _queue.get();
    boost::property_tree::ptree pt;
    pt.put("filename", "2019-04-30_14:46:22_cam02_5.mp4");
    pt.put("data", base64_file_converter("/home/pi/mqtt/mqtt/temp_saved_file_dir/2019-04-30_14:46:22_cam02_5.mp4") );
    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, pt);
    mqtt::message_ptr pubmsg = mqtt::make_message( _topic, ss.str() );

    //no json
    //mqtt::message_ptr pubmsg = mqtt::make_message( _topic, base64_file_converter("/home/pi/mqtt/mqtt/temp_saved_file_dir/2019-04-30_14:46:22_cam02_5.mp4") );
    ////////////////////////////////////////////////////////////////
    std::cout<<"[Publisher::"<<__func__<<"]"<<". publishing...\n";
    try{
      _client.publish(pubmsg, nullptr, _listener);
      while( !_listener.is_done() ) 
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    catch(const mqtt::exception& exc) {
      std::cerr << exc.what() << std::endl;
      return ;
    }
      }
}
