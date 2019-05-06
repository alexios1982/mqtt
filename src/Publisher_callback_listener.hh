#ifndef _PUBLISHER_CALLBACK_LISTENER_
#define _PUBLISHER_CALLBACK_LISTENER_

#include<iostream>
#include<atomic>
//////////////////////////////////////////////////////////////////////////////////////////
/**
 * A callback class for use with the main MQTT client.
 */
class Publisher_callback: public virtual mqtt::callback{
  
public:
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

/**
 * A base action listener.
 */
class Publisher_action_listener : public virtual mqtt::iaction_listener
{
protected:
  void on_failure(const mqtt::token& tok) override {
    std::cout <<"[Publisher_action_listener::"<<__func__<<"]" << "\tListener failure for token: "
	      << tok.get_message_id() << std::endl;
  }

  void on_success(const mqtt::token& tok) override {
    std::cout<<"[Publisher_action_listener::"<<__func__<<"]" << "\tListener success for token: "
	      << tok.get_message_id() << std::endl;
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
