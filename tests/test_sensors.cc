#include "mqtt/async_client.h"
#include "Test_zigbee_to_mqtt_subscriber_action_listener.hh"
#include <string>


///This test is intended to test the operation of the sensors.
///The sensor must first be paired and, during pairing,
//its device_id must be registered and supplied as the first and only argument of the program
///Remember that each new sensor will publish on a topic like "zigbee2mqtt/device_id"
//where zigbee2mqtt is the root set in the zigbee2mqtt configuration file called configuration.yaml

const std::string ZIGBEE_SERVER_ADDRESS	{ "tcp://localhost:1883" };
const std::string ZIGBEE_CLIENT_ID		{ "zigbee_subscribe_client" };
//# is a wildcard to subscribe to all zigbee2mqtt topics
//0x00158d000202c411 is  the second level topic created by the motion-sensor
//0x00158d0001cc99b3 is the second level topic created by the contact-sensor
//const std::string ZIGBEE_TOPIC 		{ "zigbee2mqtt/0x00158d000202c411" };

const int QOS = 1;

int main(int argc, char* argv[]){

  std::string device_id = "0x00158d000202c411";
  
  if(argc < 2){
    std::cout << "No device-id supplied.\n";
    std::cout << "Using the default one associated to motion-sensor: 0x00158d000202c411.\n";
    std::cout << "Other available device id are:\n";
    std::cout << "contact-sensor ======> 0x00158d0001cc99b3" << std::endl;
  }
  //in case of more than one command-line arguments, we only consider the first
  else{
    device_id = argv[1];
  }
  std::string ZIGBEE_TOPIC( std::string("zigbee2mqtt/") + device_id );
  mqtt::connect_options zigbee_conn_opts{"hub_raspberry", "hub_raspberry"};
  
  zigbee_conn_opts.set_keep_alive_interval(20);
  zigbee_conn_opts.set_clean_session(true);

  mqtt::async_client zigbee_client(ZIGBEE_SERVER_ADDRESS, ZIGBEE_CLIENT_ID);
  Test_zigbee_to_mqtt_subscriber_action_listener zig_callback_listener(zigbee_client,
							zigbee_conn_opts,
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

    // Just block till user tells us to quit.
  while (std::tolower(std::cin.get()) != 'q')
    ;

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
