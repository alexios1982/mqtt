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

const std::string PUBLISHER_SERVER_ADDRESS   { "ssl://a2x7fkplgngdk8-ats.iot.us-east-1.amazonaws.com:8883" };
const std::string PUBLISHER_CLIENT_ID        { "publisher_client" };
const std::string PUBLISHER_TOPIC            { "Notifications" };


int QOS = 1;

int main(int argc, char* argv[]){
  boost::ignore_unused(argc);
  boost::ignore_unused(argv);

  
  // mqtt::connect_options conn_opts{"FlespiToken R4XF03Rp3KStynVTDRrOuju7odMxQjYxdJ32DKhuiYNGbwnEbEgvMBt0C3nid9Fe", "bEEvvyn1bxDFw0-s"};
  // mqtt::ssl_options sslopts;
  // conn_opts.set_ssl(sslopts);

  mqtt::connect_options conn_opts{};
  mqtt::ssl_options sslopts("AmazonRootCA1.pem", "33ac0ac8dc-certificate.pem.crt", "33ac0ac8dc-private.pem.key", "", "", true);
  conn_opts.set_ssl(sslopts);


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

  boost::property_tree::ptree pt;
  //pt.put("filename", "2019-06-19_18:20:56_cam01_4.mp4");
  pt.put("filename", "2019-04-30_14:46:22_cam02_5.jpeg");
  //pt.put( "data", base64_file_converter("/home/pi/mqtt/mqtt/temp_saved_file_dir/2019-06-19_18:20:56_cam01_4.mp4") );
  //pt.put( "data", base64_file_converter("/home/pi/mqtt/mqtt/temp_saved_file_dir/ex_file_300x400.jpg") );
  pt.put( "data", base64_file_converter("/home/pi/mqtt/mqtt/temp_saved_file_dir/2019-04-30_14:46:22_cam02_5.jpeg") );
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, pt);
  mqtt::message_ptr pubmsg = mqtt::make_message( PUBLISHER_TOPIC, ss.str() );
  Delivery_action_listener delivery_listener;

  char user_input{};
  while (std::cout << "\nPress any key to send a message or \'q\' to exit:\t"
	 && std::cin >> user_input && std::tolower(user_input) != 'q'){

    try {
      std::cout << "pubmsg size is: " << ( pubmsg->to_string() ).size() << '\n';
      publisher_client.publish (pubmsg, nullptr, delivery_listener);
      while ( !(delivery_listener.is_done() ) ) 
	std::this_thread::sleep_for ( std::chrono::milliseconds(100) );
    }
    catch (const mqtt::exception& exc) {
      std::cerr << exc.what() << std::endl;
      return 1;
    }
    //let's wait some time before sending the other message
    std::this_thread::sleep_for ( std::chrono::milliseconds(1000) );
  }

  return 0;
}

