#ifndef _PUBLISHER_
#define _PUBLISHER_

#include "Synchronized_queue.hh"
#include "mqtt/async_client.h"
#include "Publisher_callback_listener.hh"
#include "Utils.hh"

class Publisher{
  mqtt::async_client &_client;
  const std::string &_topic;
  Delivery_action_listener &_listener;
  Synchronized_queue<mqtt::const_message_ptr> &_queue;
  Dir_handler &_dir_handler;
public:
  Publisher(mqtt::async_client &client,
	    const std::string &topic,
	    Delivery_action_listener &listener,
	    Synchronized_queue<mqtt::const_message_ptr> &queue,
	    Dir_handler &dir_handler);
  void run();
};

#endif
