#define DEBUG

#ifndef _RISK_FACTOR_STATES_
#define _RISK_FACTOR_STATES_

//front-end
#include <boost/msm/front/state_machine_def.hpp>
#include "Utils.hh"

namespace msm = boost::msm;

// The list of Risk_factor states

struct Waiting_for_risk : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Waiting_for_risk::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Evaluating_risk : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Evaluating_risk::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Low_risk : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Low_risk::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Medium_risk : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Medium_risk::" << __func__ << "]. " << reset << std::endl);
  }
};

struct High_risk : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[High_risk::" << __func__ << "]. " << reset << std::endl);
  }
};

#endif
