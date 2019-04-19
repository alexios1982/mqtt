#include "Publisher.hh"
#include <iostream>
#include <thread>

Publisher::Publisher(mqtt::async_client &client,
		     const std::string &topic,
		     Delivery_action_listener &listener,
		     Synchronized_queue<mqtt::const_message_ptr> &queue):
  _client{client},
  _topic(topic),
  _listener{listener},
  _queue(queue){}

void Publisher::run(){
  while(true){
    mqtt::const_message_ptr msg = _queue.get();
    mqtt::message_ptr pubmsg = mqtt::make_message( _topic, msg->get_payload() );
    std::cout<<"publishing...\n";
    try{
      _client.publish(pubmsg, nullptr, _listener);
      while (!_listener.is_done()) 
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    catch(const mqtt::exception& exc) {
      std::cerr << exc.what() << std::endl;
      return ;
    }
  }
}
