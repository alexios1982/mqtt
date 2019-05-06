/////////////////////////////////////////////////////////////////////////////
// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.
#ifndef _SUBSCRIBER_CALLBACK_LISTENER_
#define _SUBSCRIBER_CALLBACK_LISTENER_

#include "mqtt/async_client.h"
#include <iostream>
#include "Synchronized_queue.hh"
#include <boost/property_tree/json_parser.hpp>

const int N_RETRY_ATTEMPTS = 5;

class Action_listener: public virtual mqtt::iaction_listener{
  std::string _name;

  void on_failure(const mqtt::token &token) override{
    std::cout << _name << " failure";
    if(token.get_message_id() != 0)
      std::cout << " for token: [" << token.get_message_id() << "]" << std::endl;
    std::cout << std::endl;
  }

  void on_success(const mqtt::token &token) override{
    std::cout << _name << " success";
    if (token.get_message_id() != 0)
      std::cout << " for token: [" << token.get_message_id() << "]" << std::endl;
    auto top = token.get_topics();
    if (top && !top->empty())
      std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
    std::cout << std::endl;
  }
public:
  Action_listener(const std::string &name): _name(name){}
};


class Subscriber_callback_listener: public virtual mqtt::callback,
				    public virtual mqtt::iaction_listener{

  // Counter for the number of connection retries
  int _n_retry;
  // The MQTT client
  mqtt::async_client &_client;
  // Options to use if we need to reconnect
  mqtt::connect_options &_conn_opts;
  // An action listener to display the result of actions.
  Action_listener _sub_listener;
  Synchronized_queue<mqtt::const_message_ptr> &_queue;
  const std::string &_topic;
  const std::string &_client_id;
  int _qos;

  // This demonstrates manually reconnecting to the broker by calling
  // connect() again. This is a possibility for an application that keeps
  // a copy of it's original connect_options, or if the app wants to
  // reconnect with different options.
  // Another way this can be done manually, if using the same options, is
  // to just call the async_client::reconnect() method.
  void reconnect() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    try {
      _client.connect(_conn_opts, nullptr, *this);
    }
    catch (const mqtt::exception& exc) {
      std::cerr << "Error: " << exc.what() << std::endl;
      exit(1);
    }
  }

  // Re-connection failure
  void on_failure(const mqtt::token& tok) override {
    std::cout << "Connection attempt failed" << std::endl;
    if (++_n_retry > N_RETRY_ATTEMPTS)
      exit(1);
    reconnect();
  }

  // (Re)connection success
  // Either this or connected() can be used for callbacks.
  void on_success(const mqtt::token& tok) override {}

  // (Re)connection success
  void connected(const std::string& cause) override {
    std::cout << "\nConnection success" << std::endl;
    std::cout << "\nSubscribing to topic '" << _topic << "'\n"
	      << "\tfor client " << _client_id
	      << " using QoS" << _qos << "\n"
	      << "\nPress Q<Enter> to quit\n" << std::endl;

    _client.subscribe(_topic, _qos, nullptr, _sub_listener);
  }

  // Callback for when the connection is lost.
  // This will initiate the attempt to manually reconnect.
  void connection_lost(const std::string& cause) override {
    std::cout << "\nConnection lost" << std::endl;
    if (!cause.empty())
      std::cout << "\tcause: " << cause << std::endl;

    std::cout << "Reconnecting..." << std::endl;
    _n_retry = 0;
    reconnect();
  }

  // Callback for when a message arrives.
  void message_arrived(mqtt::const_message_ptr msg) override {
    std::string topic = msg->get_topic();
    if (!topic.compare(26,4, "99b3")){
      std::cout<<"is the door sensor"<<std::endl;
      parse_door_sensor_message(msg);
    }
  }

  void delivery_complete(mqtt::delivery_token_ptr token) override {}

  void parse_door_sensor_message(mqtt::const_message_ptr msg){
    //solution to double notification problems
    //we discard the second message because sensor port always
    //sends two messages for each event: closed door and opened door
    static int previous_contact = -1;
    std::string payload(msg->to_string());
    std::stringstream ss;
    
    std::cout << "\tpayload: '" << payload << "'\n" << std::endl;
    ss << payload;

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);
    //contact value is true or false, but we want to convert to an int value
    int actual_contact = pt.get<bool>("contact");
    std::cout << "actual contact: " << actual_contact << std::endl;	
    if(actual_contact != previous_contact){
      _queue.push(msg);
      previous_contact = actual_contact;
    }
  }
  
public:
  Subscriber_callback_listener(mqtt::async_client &client,
			       mqtt::connect_options &conn_opts,
			       Synchronized_queue<mqtt::const_message_ptr> &queue,
			       const std::string &topic,
			       const std::string &client_id,
			       int qos)
    : _n_retry{0},
      _client{client},
      _conn_opts{conn_opts},
      _sub_listener{"Subscription"},
      _queue{queue},
      _topic{topic},
      _client_id{client_id},
      _qos{qos}{}
};


#endif
