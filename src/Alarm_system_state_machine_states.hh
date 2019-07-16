#define DEBUG

#ifndef _ALARM_SYSTEM_STATES_
#define _ALARM_SYSTEM_STATES_

//front-end
#include <boost/msm/front/state_machine_def.hpp>
#include "Utils.hh"

namespace msm = boost::msm;

// The list of Alarm_system states

struct Init : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Init::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Waiting_for_configuration : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Waiting_for_configuration::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Green_alarm : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Green_alarm::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Orange_alarm_notified : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Orange_alarm_notified::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Red_alarm_notified : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Red_alarm_notified::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Exit : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Exit::" << __func__ << "]. " << reset << std::endl);
  }  
};

struct Red_alarm_actuation : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Red_alarm_actuation::" << __func__ << "]. " << reset << std::endl);
  }
};

#endif
