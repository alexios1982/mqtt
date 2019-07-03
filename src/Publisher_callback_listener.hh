#ifndef _PUBLISHER_CALLBACK_LISTENER_
#define _PUBLISHER_CALLBACK_LISTENER_

#include<iostream>
#include<atomic>
#include "Utils.hh"
#include <boost/core/ignore_unused.hpp>

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * A callback class for use with the main MQTT client.
 */
class Publisher_callback: public virtual mqtt::callback{
  
public:
  void connection_lost(const std::string &cause) override {
    std::cerr << error << "\n[Publisher_callback::"
	      << __func__ << "]. " << reset ; 
    std::cerr << "\nConnection lost" << '\n';
    if (!cause.empty())
      std::cerr << "\tcause: " << cause << std::endl;
  }

  void delivery_complete(mqtt::delivery_token_ptr tok) override {
    boost::ignore_unused(tok);
    D(std::cout << info << "\n[Publisher_callback::" << __func__ << "]. " << reset
      << "\tDelivery complete for token: "
      << (tok ? tok->get_message_id() : -1) << std::endl;)
  }
};


/////////////////////////////////////////////////////////////////////////////

/**
 * A base action listener.
 */
class Publisher_action_listener : public virtual mqtt::iaction_listener
{
protected:
  void on_failure(const mqtt::token& tok) override {
    std::cerr << error << "[Publisher_action_listener::"<< __func__ <<"]." << reset
	      << "\tListener failure for token: "
	      << tok.get_message_id() << std::endl;
  }

  void on_success(const mqtt::token& tok) override {
    boost::ignore_unused(tok);
    D(std::cout << info << "[Publisher_action_listener::" << __func__ << "]." << reset
      << "\tListener success for token: "
      << tok.get_message_id() << std::endl);
  }
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

/**
 * A derived action listener for publish events.
 */
class Delivery_action_listener: public Publisher_action_listener{
  std::atomic<bool> _done;

  void on_failure(const mqtt::token& tok) override {
    Publisher_action_listener::on_failure(tok);
    _done = true;
  }

  void on_success(const mqtt::token& tok) override {
    Publisher_action_listener::on_success(tok);
    _done = true;
  }

public:
  Delivery_action_listener() : _done(false) {}
  bool is_done() const { return _done; }
};

/////////////////////////////////////////////////////////////////////////////


#endif
