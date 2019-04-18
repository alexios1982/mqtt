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

using namespace std;

const string ZIGBEE_SERVER_ADDRESS	{ "tcp://localhost:1883" };
const string ZIGBEE_CLIENT_ID		{ "zigbee_subscribe_client" };
//# is a wildcard to subscribe to all zigbee2mqtt topics
//0x00158d0001cc99b3 is  the second levele created by the open-door sensor
const string ZIGBEE_TOPIC 		{ "zigbee2mqtt/0x00158d0001cc99b3" };


const std::string ABLY_SERVER_ADDRESS   { "ssl://mqtt.ably.io:8883" };
const std::string ABLY_CLIENT_ID        { "ably_publish_client" };
const std::string ABLY_TOPIC            { "notifications" };

const auto PUBLISHER_TIMEOUT = std::chrono::seconds(10);

const int  QOS = 1;

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * A callback class for use with the main MQTT client.
 */
class ably_subscriber_callback : public virtual mqtt::callback
{
public:
  void connection_lost(const std::string& cause) override {
    std::cout << "\nConnection lost" << std::endl;
    if (!cause.empty())
      std::cout << "\tcause: " << cause << std::endl;
  }

  void delivery_complete(mqtt::delivery_token_ptr tok) override {
    std::cout << "\tDelivery complete for token: "
	      << (tok ? tok->get_message_id() : -1) << std::endl;
  }
};

/////////////////////////////////////////////////////////////////////////////

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
  mqtt::async_client ably_client(ABLY_SERVER_ADDRESS, ABLY_CLIENT_ID);
  ably_subscriber_callback ab_cb;
  ably_client.set_callback(ab_cb);

  // mqtt::connect_options ably_conn_opts{"Wb7GPA.RVtWzA", "bEEvvyn1bxDFw0-s"};  
  // mqtt::ssl_options ably_sslopts;
  // ably_conn_opts.set_ssl(ably_sslopts);

  // try {
  //   cout << "\nConnecting to the ABLY MQTT server..." << endl;
  //   mqtt::token_ptr conntok = ably_client.connect(ably_conn_opts);
  //   cout << "Waiting for the connection..." << endl;
  //   conntok->wait();
  //   cout << "  ... CONNECTION TO ABLY OK" << endl;
  // }
  // catch (const mqtt::exception& exc) {
  //   std::cerr << "\nERROR: Unable to connect to ABLY MQTT  server: '" <<endl;
  //   cerr << exc.what() << endl;
  //   return 1;
  // }
  
  // Just block till user tells us to quit.
  while (std::tolower(std::cin.get()) != 'q')
    ;

  // cout << "\nSending message..." << endl;
  // mqtt::message_ptr pubmsg = mqtt::make_message(ABLY_TOPIC, "ciao ciao");
  // pubmsg->set_qos(QOS);
  // ably_client.publish(pubmsg)->wait_for(PUBLISHER_TIMEOUT);
  // cout << "  ...OK" << endl;
  
  // Disconnect
  try {
    std::cout << "\nDisconnecting from the ZIGBEE MQTT server..." << std::flush;
    zigbee_client.disconnect()->wait();
    std::cout << "OK" << std::endl;
  }
  catch (const mqtt::exception& exc) {
    std::cerr << exc.what() << std::endl;
    return 1;
  }
  
  return 0;
}

