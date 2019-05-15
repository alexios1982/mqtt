#include "mqtt/async_client.h"
#include "Subscriber_callback_listener.hh"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Utils.hh"
#include <base64.hh> //for base64_file_converter
#include <boost/core/ignore_unused.hpp>
#include "Publisher_callback_listener.hh"
#include "Test_subscriber_action_listener.hh"
#include <iostream>

const std::string PUBLISHER_SERVER_ADDRESS   { "ssl://mqtt.flespi.io:8883" };
const std::string PUBLISHER_CLIENT_ID        { "publisher_client" };
const std::string PUBLISHER_TOPIC            { "Notifications" };

const std::string SUBSCRIBER_SERVER_ADDRESS  {"ssl://mqtt.flespi.io:8883"};
const std::string SUBSCRIBER_CLIENT_ID       {"subscriber_client"};
const std::string SUBSCRIBER_TOPIC           {"Response"};

int QOS = 1;

int main(int argc, char* argv[]){
  boost::ignore_unused(argc);
  boost::ignore_unused(argv);

  
  mqtt::connect_options conn_opts{"FlespiToken R4XF03Rp3KStynVTDRrOuju7odMxQjYxdJ32DKhuiYNGbwnEbEgvMBt0C3nid9Fe", "bEEvvyn1bxDFw0-s"};
  mqtt::ssl_options sslopts;
  conn_opts.set_ssl(sslopts);


  mqtt::async_client subscriber(SUBSCRIBER_SERVER_ADDRESS,
				SUBSCRIBER_CLIENT_ID);
  Test_subscriber_action_listener action_listener(subscriber, 
						  conn_opts,
						  SUBSCRIBER_TOPIC,
						  SUBSCRIBER_CLIENT_ID,
						  QOS);
  subscriber.set_callback(action_listener);

  // Start the connection.
  // When completed, the callback will subscribe to topic.  
  try {
    std::cout << "Connecting to the SUBSCRIBER MQTT server..." << std::flush;
    subscriber.connect(conn_opts, nullptr, action_listener);
  }
  catch (const mqtt::exception&) {
    std::cerr << "\nERROR: Unable to connect to the SUBSCRIBER MQTT server: '"
	      << SUBSCRIBER_SERVER_ADDRESS << "'" << std::endl;
    return 1;
  }

  //Create the async_client object
  mqtt::async_client publisher_client(PUBLISHER_SERVER_ADDRESS, PUBLISHER_CLIENT_ID);
  Publisher_callback publisher_callback;
  publisher_client.set_callback(publisher_callback);
  //mqtt::connect_options publisher_conn_opts{"Wb7GPA.RVtWzA", "bEEvvyn1bxDFw0-s"};

  // mqtt::connect_options publisher_conn_opts{"FlespiToken R4XF03Rp3KStynVTDRrOuju7odMxQjYxdJ32DKhuiYNGbwnEbEgvMBt0C3nid9Fe", "bEEvvyn1bxDFw0-s"};
  // mqtt::ssl_options publisher_sslopts;
  // publisher_conn_opts.set_ssl(publisher_sslopts);

  try {
    std::cout << "\nConnecting to the PUBLISHER MQTT server..." << std::endl;
    mqtt::token_ptr conntok = publisher_client.connect(conn_opts);
    std::cout << "Waiting for the connection..." << std::endl;
    conntok->wait();
    std::cout << "  ... CONNECTION TO PUBLISHER OK" << std::endl;
  }
  catch (const mqtt::exception& exc) {
    std::cerr << "\nERROR: Unable to connect to PUBLISHER MQTT  server: '" << std::endl;
    std::cerr << exc.what() << std::endl;
    return 1;
  }

  Dir_handler dir_handler{"/home/pi/gstreamer/multifiles_saving"};

  Delivery_action_listener delivery_listener;

  int iter = 0, n_of_sending = 12;
  while( iter < n_of_sending){
    boost::property_tree::ptree pt;
    if( !dir_handler.exists() ){
      std::cerr << "[test_multiple_sending::" << __func__ << "]. " << "no directory with mp4 file. " << std::endl; 
      exit(-1);
    }
    Dir_handler::Time_path_pair to_send = dir_handler.get_last_modified_file(".mp4");
    std::string to_send_filename = ( (to_send.second).filename() ).string();
    std::time_t now;
    std::time (&now);
    std::cout << "The current local time is: " << std::ctime(&now)  << '\n';
    std::cout << "[Publisher::" << __func__ << "]. " << "video " << (iter + 1) << " to publish is " << to_send_filename << '\n';
    //let's wait until the video chunk to send is finished:
    //the condition for this to happen is the presence of a next video chunk 
    while(true){
      Dir_handler::Time_path_pair next = dir_handler.get_last_modified_file(".mp4");
      if( ( (next.second).filename() ).string() != to_send_filename )
	break;
      std::this_thread::sleep_for ( std::chrono::milliseconds(500) ); 
    }
    
    pt.put("filename", to_send_filename);
    pt.put( "data", base64_file_converter( (to_send.second).string() ) );
    
    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, pt);
    mqtt::message_ptr pubmsg = mqtt::make_message( PUBLISHER_TOPIC, ss.str() );
    
    //no json
    //mqtt::message_ptr pubmsg = mqtt::make_message( _topic, base64_file_converter("/home/pi/mqtt/mqtt/temp_saved_file_dir/2019-04-30_14:46:22_cam02_5.mp4") );
    ////////////////////////////////////////////////////////////////
    std::cout << "[test_multiple_sending::" << __func__ << "]. " << "publishing ..." << '\n';
    try{
      publisher_client.publish(pubmsg, nullptr, delivery_listener);
      while( !delivery_listener.is_done() ) 
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    catch(const mqtt::exception& exc) {
      std::cerr << exc.what() << std::endl;
      exit(-1) ;
    }
    ++iter;
  } //END OF WHILE LOOP FOR N_OF_SENDING
  return 0;
}

