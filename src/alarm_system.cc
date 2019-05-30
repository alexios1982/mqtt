// async_consume.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// This application is an MQTT consumer/subscriber using the C++
// asynchronous client interface, employing the  to receive messages
// and status updates.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker.
//  - Subscribing to a topic
//  - Receiving messages through the synchronous queuing API
//

/*******************************************************************************
 * Copyright (c) 2013-2017 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/async_client.h"
#include <boost/core/ignore_unused.hpp>
#include "Subscriber_callback_listener.hh"
#include "Publisher.hh"
#include "Notification_logic_controller.hh"

const std::string ZIGBEE_SERVER_ADDRESS	{ "tcp://localhost:1883" };
const std::string ZIGBEE_CLIENT_ID		{ "zigbee_subscribe_client" };
//# is a wildcard to subscribe to all zigbee2mqtt topics
//0x00158d0001cc99b3 is  the second levele created by the open-door sensor
const std::string ZIGBEE_TOPIC 		{ "zigbee2mqtt/0x00158d0001cc99b3" };

//const std::std::string PUBLISHER_SERVER_ADDRESS   { "ssl://mqtt.ably.io:8883" };
const std::string PUBLISHER_SERVER_ADDRESS   { "ssl://mqtt.flespi.io:8883" };
const std::string PUBLISHER_CLIENT_ID        { "publisher_client" };
const std::string PUBLISHER_TOPIC            { "Notifications" };



const int  QOS = 1;

int main(int argc, char* argv[])
{
  boost::ignore_unused(argc);
  boost::ignore_unused(argv);
  
  mqtt::connect_options zigbee_conn_opts{"hub_raspberry", "hub_raspberry"};
  Synchronized_queue<mqtt::const_message_ptr> queue;
  
  zigbee_conn_opts.set_keep_alive_interval(20);
  zigbee_conn_opts.set_clean_session(true);

  mqtt::async_client zigbee_client(ZIGBEE_SERVER_ADDRESS, ZIGBEE_CLIENT_ID);
  Subscriber_callback_listener zig_callback_listener(zigbee_client,
						     zigbee_conn_opts,
						     queue,
						     ZIGBEE_TOPIC,
						     ZIGBEE_CLIENT_ID,
						     QOS);
  zigbee_client.set_callback(zig_callback_listener);
  
  
  // Start the connection.
  // When completed, the callback will subscribe to topic.  
  try {
    std::cout << "Connecting to the ZIGBEE MQTT server..." << std::flush;
    zigbee_client.connect(zigbee_conn_opts, nullptr, zig_callback_listener);
  }
  catch (const mqtt::exception&) {
    std::cerr << "\nERROR: Unable to connect to MQTT server: '"
	      << ZIGBEE_SERVER_ADDRESS << "'" << std::endl;
    return 1;
  }

  
  //Create the async_client object
  mqtt::async_client publisher_client(PUBLISHER_SERVER_ADDRESS, PUBLISHER_CLIENT_ID);
  Publisher_callback publisher_callback;
  publisher_client.set_callback(publisher_callback);
  //mqtt::connect_options publisher_conn_opts{"Wb7GPA.RVtWzA", "bEEvvyn1bxDFw0-s"};
  mqtt::connect_options publisher_conn_opts{"FlespiToken R4XF03Rp3KStynVTDRrOuju7odMxQjYxdJ32DKhuiYNGbwnEbEgvMBt0C3nid9Fe", "bEEvvyn1bxDFw0-s"};
  mqtt::ssl_options publisher_sslopts;
  publisher_conn_opts.set_ssl(publisher_sslopts);

  try {
    std::cout << "\nConnecting to the PUBLISHER MQTT server..." << std::endl;
    mqtt::token_ptr conntok = publisher_client.connect(publisher_conn_opts);
    std::cout << "Waiting for the connection..." << std::endl;
    conntok->wait();
    std::cout << "  ... CONNECTION TO SERVER IS OK" << std::endl;
  }
  catch (const mqtt::exception& exc) {
    std::cerr << "\nERROR: Unable to connect to PUBLISHER MQTT server: '" <<std::endl;
    std::cerr << exc.what() << std::endl;
    return 1;
  }

  Delivery_action_listener publisher_listener;
  Dir_handler dir_handler{"/home/pi/gstreamer/multifiles_saving"};
  Publisher publisher(publisher_client,
		      PUBLISHER_TOPIC,
		      publisher_listener,
		      queue,
		      dir_handler);
  std::thread t( std::bind(&Publisher::run, &publisher) );
  t.detach();
  // Just block till user tells us to quit.
  while (std::tolower(std::cin.get()) != 'q')
    ;

  //Disconnecting the subscriber
  try {
    std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
    publisher_client.disconnect()->wait();
    std::cout << "OK" << std::endl;
  }
  catch (const mqtt::exception& exc) {
    std::cerr << exc.what() << std::endl;
    return 1;
  }
    //Disconnecting the subscriber
  try {
    std::cout << "\nDisconnecting from the ZIGBEE2MQTT server..." << std::flush;
    zigbee_client.disconnect()->wait();
    std::cout << "OK" << std::endl;
  }
  catch (const mqtt::exception& exc) {
    std::cerr << exc.what() << std::endl;
    return 1;
  }
  
  return 0;
}

