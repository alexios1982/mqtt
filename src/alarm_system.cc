#define DEBUG

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
//#include "Sensor.hh"

//
// Testing utilities.
//
static char const* const state_names[] = { //"Init",
					   "Waiting_for_configuration",
					   "Green_alarm",
					   "Orange_alarm_notified",
					   "Red_alarm_notified",
					   "Red_alarm_actuation",
					   "Exit",
					   "Waiting_for_risk",
					   "Evaluating_risk",
					   "Low_risk",
					   "Medium_risk",
					   "High_risk",
					   "None",
					   "Extern",
					   "Intern",
					   "Reserved",
					   "Idle",
					   "Waiting_for_ai_response"};

void print_current_state(Alarm_system const &alarm_system){
  D(std::cout << warning << __func__ << reset << std::endl);
   // we have now several active states, which we show
  for (unsigned int i = 0;
       i < Alarm_system::nr_regions::value;
       ++i){
      D(std::cout << warning << "::print_current_states. Current states -> " << reset << state_names[alarm_system.current_state()[i]] << std::endl);
    }
}

const std::string ZIGBEE_SERVER_ADDRESS	{ "tcp://localhost:1883" };
const std::string ZIGBEE_SUBSCRIBER_ID { "zigbee_subscribe_client" };
//# is a wildcard to subscribe to all zigbee2mqtt topics
//0x00158d0001cc99b3 is the second level topic created by the open-door sensor
//0x00158d000202c411 is the second level topic created by the motion sensor
//const std::string ZIGBEE_SUBSCRIBER_TOPIC { "zigbee2mqtt/0x00158d0001cc99b3" };
//const std::string ZIGBEE_SUBSCRIBER_TOPIC_2 { "zigbee2mqtt/0x00158d000202c411" };
//to subscribe to all topics created by sensors
const std::string ZIGBEE_SUBSCRIBER_ALL_TOPICS { "zigbee2mqtt/#" };

//const std::std::string PUBLISHER_SERVER_ADDRESS { "ssl://mqtt.ably.io:8883" };
//const std::string REMOTE_SERVER_ADDRESS { "ssl://mqtt.flespi.io:8883" };
const std::string REMOTE_SERVER_ADDRESS { "ssl://a2x7fkplgngdk8-ats.iot.us-east-1.amazonaws.com:8883" };
const std::string REMOTE_PUBLISHER_ID { "remote_publisher_client" };
const std::string REMOTE_SUBSCRIBER_ID { "remote_subscriber_client" };
const std::string REMOTE_PUBLISHER_TOPIC { "User_Consitalia_1_aws/Notifications" };
const std::string REMOTE_SUBSCRIBER_TOPIC {"User_Consitalia_1_aws/Response"};


const int QOS = 1;

int main(int argc, char* argv[]){
    
  boost::ignore_unused(argc);
  boost::ignore_unused(argv);

  //here we have to build the map that will associates a sensor to a cam
  //and the map that will associate the cam to its chunks folder
  //lets' use the last 4 chars of sensor id to improve research speed in map
  //if a sensor is present in the sensor_cam map, we have to send a rich
  //notification, otherwise we will send a classify notification
  //TODO
  //queste dovremmo riempirle in fase di configurazione della macchina a stati
  std::map<std::string, std::string> sensor_cam = { {"01cc99b3", "cam02"} };
  std::map<std::string, std::string> cam_path = { {"cam02", "/home/pi/gstreamer/multifiles_saving"} };
  /////////////////////////////////////////////////////////////////////////

  // std::map< std::string, Sensor<Ext_door_open_sensor_sig> > ext_sensors_map;

  // Ext_door_open_sensor_sig ext_door_open_sensor_sig;
  // ext_door_open_sensor_sig.cam = "cam02";
  
  // //Sensor<Ext_door_open_sensor_sig> sensor1 = {"01cc99b3", ext_door_open_sensor_sig, "ext", "garden"};
  // ext_sensors_map["01cc99b3"] = {"01cc99b3", Sensor_type::CONTACT, ext_door_open_sensor_sig, Ring_class::EXTERNAL, "garden"};
  
  mqtt::connect_options zigbee_conn_opts{"hub_raspberry", "hub_raspberry"};
  Synchronized_queue<mqtt::const_message_ptr> queue;
  
  zigbee_conn_opts.set_keep_alive_interval(20);
  zigbee_conn_opts.set_clean_session(true);

  mqtt::async_client zigbee_subscriber(ZIGBEE_SERVER_ADDRESS, ZIGBEE_SUBSCRIBER_ID);
  Subscriber_callback_listener zig_callback_listener(zigbee_subscriber,
						     zigbee_conn_opts,
						     queue,
						     //ZIGBEE_SUBSCRIBER_TOPIC,
						     ZIGBEE_SUBSCRIBER_ALL_TOPICS,
						     ZIGBEE_SUBSCRIBER_ID,
						     QOS);
  zigbee_subscriber.set_callback(zig_callback_listener);

  // Start the connection.
  // When completed, the callback will subscribe to topic.  
  try {
    std::cout << "Connecting to the ZIGBEE MQTT server..." << std::flush;
    zigbee_subscriber.connect(zigbee_conn_opts, nullptr, zig_callback_listener);
  }
  catch (const mqtt::exception&) {
    std::cerr << "\nERROR: Unable to connect to MQTT server: '"
	      << ZIGBEE_SERVER_ADDRESS << "'" << std::endl;
    return 1;
  }

  // mqtt::connect_options remote_conn_opts{"FlespiToken R4XF03Rp3KStynVTDRrOuju7odMxQjYxdJ32DKhuiYNGbwnEbEgvMBt0C3nid9Fe", "bEEvvyn1bxDFw0-s"};
  // mqtt::ssl_options remote_sslopts;
  // remote_conn_opts.set_ssl(remote_sslopts);

  mqtt::connect_options remote_conn_opts{};
  mqtt::ssl_options remote_sslopts("AmazonRootCA1.pem", "33ac0ac8dc-certificate.pem.crt", "33ac0ac8dc-private.pem.key", "", "", true);
  remote_conn_opts.set_ssl(remote_sslopts);
  

  mqtt::async_client remote_publisher(REMOTE_SERVER_ADDRESS, REMOTE_PUBLISHER_ID);
  Publisher_callback publisher_callback;
  remote_publisher.set_callback(publisher_callback);
  //mqtt::connect_options publisher_conn_opts{"Wb7GPA.RVtWzA", "bEEvvyn1bxDFw0-s"};

  try {
    std::cout << "\nConnecting to the PUBLISHER MQTT server..." << std::endl;
    mqtt::token_ptr conntok = remote_publisher.connect(remote_conn_opts);
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
  Publisher publisher(remote_publisher,
		      REMOTE_PUBLISHER_TOPIC,
		      publisher_listener,
		      queue,
		      dir_handler);


  mqtt::async_client remote_subscriber(REMOTE_SERVER_ADDRESS, REMOTE_SUBSCRIBER_ID);
  Subscriber_callback_listener remote_callback_listener(remote_subscriber,
							remote_conn_opts,
							queue,
							REMOTE_SUBSCRIBER_TOPIC,
							REMOTE_SUBSCRIBER_ID,
							QOS);
  remote_subscriber.set_callback(remote_callback_listener);

  // Start the connection.
  // When completed, the callback will subscribe to topic.  
  try {
    std::cout << "Connecting to the REMOTE MQTT server..." << std::flush;
    remote_subscriber.connect(remote_conn_opts, nullptr, remote_callback_listener);
  }
  catch (const mqtt::exception&) {
    std::cerr << "\nERROR: Unable to connect to REMOTE MQTT server: '"
	      << REMOTE_SERVER_ADDRESS << "'" << std::endl;
    return 1;
  }

  // std::thread t( std::bind(&Publisher::run, &publisher) );
  // t.detach();
  Area_protection area_protection;

  std::string configuration_file = "some_configuration_file";
  
  Notification_logic_controller notification_logic_controller{area_protection, queue, publisher, sensor_cam, cam_path};
  notification_logic_controller.start();

  notification_logic_controller.load_configuration(configuration_file);
  
  D(  
    std::cout << std::endl;
    std::cout << std::endl;
    print_current_state(notification_logic_controller);
    std::cout << std::endl;
    std::cout << std::endl;)
      
  
  std::thread t( std::bind(&Notification_logic_controller::consume_message, &notification_logic_controller) );

  // Just block till user tells us to quit.
  while (std::tolower(std::cin.get()) != 'q')
    ;

  //Disconnecting the subscriber
  try {
    std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
    remote_publisher.disconnect()->wait();
    std::cout << "OK" << std::endl;
  }
  catch (const mqtt::exception& exc) {
    std::cerr << exc.what() << std::endl;
    return 1;
  }
    //Disconnecting the subscriber
  try {
    std::cout << "\nDisconnecting from the ZIGBEE2MQTT server..." << std::flush;
    zigbee_subscriber.disconnect()->wait();
    std::cout << "OK" << std::endl;
  }
  catch (const mqtt::exception& exc) {
    std::cerr << exc.what() << std::endl;
    return 1;
  }
  
  return 0;
}

