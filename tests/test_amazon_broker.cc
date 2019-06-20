#include <iostream>
#include <string>
#include "mqtt/async_client.h"
#include <boost/core/ignore_unused.hpp>
#include <chrono>
#include <boost/core/ignore_unused.hpp>
#include "Publisher_callback_listener.hh"
#include "Test_amazon_subscriber_action_listener.hh"
///////////////////////////////////////////////////////////////////////////////
/**
 * A callback class for use with the main MQTT client.
 */
class publisher_callback : public virtual mqtt::callback{
public:

  void connected(const std::string &cause) override {
    boost::ignore_unused(cause);
    std::cout << "client connected" << std::endl;
  }
    
  void connection_lost(const std::string &cause) override {
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

const int QOS = 1;

int main(){
  //const std::string SERVER_ADDRESS { "ssl://mqtt.flespi.io:8883" };
  const std::string SERVER_ADDRESS { "ssl://a2x7fkplgngdk8-ats.iot.us-east-1.amazonaws.com:8883" };
  const std::string CLIENT_ID { "test" };
  const std::string TOPIC { "Test_mqtt" };

  //mqtt::connect_options conn_opts{"FlespiToken R4XF03Rp3KStynVTDRrOuju7odMxQjYxdJ32DKhuiYNGbwnEbEgvMBt0C3nid9Fe", "bEEvvyn1bxDFw0-s"};
  mqtt::connect_options conn_opts{};
  mqtt::ssl_options sslopts("AmazonRootCA1.pem", "33ac0ac8dc-certificate.pem.crt", "33ac0ac8dc-private.pem.key", "", "", true);
  conn_opts.set_ssl(sslopts);

  mqtt::async_client subscriber(SERVER_ADDRESS,
				CLIENT_ID);
  Test_subscriber_action_listener action_listener(subscriber, 
						  conn_opts,
						  TOPIC,
						  CLIENT_ID,
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
	      << SERVER_ADDRESS << "'" << std::endl;
    return 1;
  }
  
  // //Create the async_client object
  // mqtt::async_client publisher_client(SERVER_ADDRESS, PUBLISHER_CLIENT_ID);
  // publisher_callback publisher_callback;
  // publisher_client.set_callback(publisher_callback);

  // bool result = false;
  // try {
  //   std::cout << "\nConnecting to the broker..." << '\n';
  //   mqtt::token_ptr conntok = publisher_client.connect(conn_opts);
  //   std::cout << "Waiting for the connection..." << '\n';
  //   //I can decide to wait only a certain amount of time for
  //   //the asynchronous operation to finish. If the opeations
  //   //is not finished by the set amount if time, the result is
  //   //false, as happened if we set a short amount of time
  //   //i can decide to skip this wait() operation and
  //   //understand the success of operation by the call of callback,
  //   //e.g. publisher_callback::connected in this case 
  //   result = conntok->wait_for( std::chrono::duration<int, std::milli>(10) );
  //   std::cout << "  ... CONNECTION TO BROKER IS OK" << std::endl;
  // }
  // catch (const mqtt::exception& exc) {
  //   std::cerr << "\nERROR: Unable to connect to PUBLISHER MQTT server: '" <<std::endl;
  //   std::cerr << exc.what() << std::endl;
  //   return 1;
  // }
  // std::cout << std::boolalpha << result << std::endl;

  
  // Just block till user tells us to quit.
  while (std::tolower(std::cin.get()) != 'q')
    ;

  return 0;
}
