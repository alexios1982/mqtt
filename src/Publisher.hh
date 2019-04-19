#ifndef _PUBLISHER_
#define _PUBLISHER_

#include "Utils.hh"
#include "mqtt/async_client.h"
#include "Publisher_callback_listener.hh"

class Publisher{
  mqtt::async_client &_client;
  const std::string &_topic;
  Delivery_action_listener &_listener;
  Synchronized_queue<mqtt::const_message_ptr> &_queue;
public:
  Publisher(mqtt::async_client &client,
	    const std::string &topic,
	    Delivery_action_listener &listener,
	    Synchronized_queue<mqtt::const_message_ptr> &queue);
  void run();
};

#endif
