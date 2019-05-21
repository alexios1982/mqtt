#include "mqtt/async_client.h"
#include <iostream>
#include <sstream>
#include <boost/core/ignore_unused.hpp>

const int N_RETRY_ATTEMPTS = 5;

/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

class action_listener : public virtual mqtt::iaction_listener
{
	std::string name_;

	void on_failure(const mqtt::token& tok) override {
		std::cout << name_ << " failure";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		std::cout << std::endl;
	}

	void on_success(const mqtt::token& tok) override {
		std::cout << name_ << " success";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		auto top = tok.get_topics();
		if (top && !top->empty())
			std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
		std::cout << std::endl;
	}

public:
	action_listener(const std::string& name) : name_(name) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class Test_zigbee_to_mqtt_subscriber_action_listener : public virtual mqtt::callback,
					public virtual mqtt::iaction_listener{
  // Counter for the number of connection retries
  int nretry_;
  // The MQTT client
  mqtt::async_client& cli_;
  // Options to use if we need to reconnect
  mqtt::connect_options& connOpts_;
  // An action listener to display the result of actions.
  action_listener subListener_;
  const std::string &_topic;
  const std::string &_client_id;
  int _qos;

  // This deomonstrates manually reconnecting to the broker by calling
  // connect() again. This is a possibility for an application that keeps
  // a copy of it's original connect_options, or if the app wants to
  // reconnect with different options.
  // Another way this can be done manually, if using the same options, is
  // to just call the async_client::reconnect() method.
  void reconnect() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    try {
      cli_.connect(connOpts_, nullptr, *this);
    }
    catch (const mqtt::exception& exc) {
      std::cerr << "Error: " << exc.what() << std::endl;
      exit(1);
    }
  }

  // Re-connection failure
  void on_failure(const mqtt::token& tok) override {
    boost::ignore_unused(tok);
    std::cout << "Connection attempt failed" << std::endl;
    if (++nretry_ > N_RETRY_ATTEMPTS)
      exit(1);
    reconnect();
  }

  // (Re)connection success
  // Either this or connected() can be used for callbacks.
  void on_success(const mqtt::token& tok) override { boost::ignore_unused(tok); }

  // (Re)connection success
  void connected(const std::string& cause) override {
    boost::ignore_unused(cause);
    std::cout << "\nConnection success" << std::endl;
    std::cout << "\nSubscribing to topic '" << _topic << "'\n"
	      << "\tfor client " << _client_id
	      << " using QoS" << _qos << "\n"
	      << "\nPress Q<Enter> to quit\n" << std::endl;

    cli_.subscribe(_topic, _qos, nullptr, subListener_);
  }

  // Callback for when the connection is lost.
  // This will initiate the attempt to manually reconnect.
  void connection_lost(const std::string& cause) override {
    std::cout << "\nConnection lost" << std::endl;
    if (!cause.empty())
      std::cout << "\tcause: " << cause << std::endl;

    std::cout << "Reconnecting..." << std::endl;
    nretry_ = 0;
    reconnect();
  }

  // Callback for when a message arrives.
  void message_arrived(mqtt::const_message_ptr msg) override {
    std::time_t now;
    std::time (&now);

    std::cout << "\n[Test_subscriber_action_listener::" << __func__ << " Message arrived" << '\n';
    std::cout << "The current local time is: " << std::ctime(&now)  << '\n';
    std::cout << "\ttopic: '" << msg->get_topic() << "'" << '\n';
    std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
  }

  void delivery_complete(mqtt::delivery_token_ptr token) override { boost::ignore_unused(token); }

public:
  Test_zigbee_to_mqtt_subscriber_action_listener(mqtt::async_client& cli,
				  mqtt::connect_options& connOpts,
				  const std::string &topic,
				  const std::string &client_id,
				  int qos)
    : nretry_(0),
      cli_(cli),
      connOpts_(connOpts),
      subListener_("Subscription"),
      _topic(topic),
      _client_id(client_id),
      _qos(qos){}
};
