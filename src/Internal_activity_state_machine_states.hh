#define DEBUG

#ifndef _INTERNAL_ACTIVITY_STATES_
#define _INTERNAL_ACTIVITY_STATES_ 

//front-end
#include <boost/msm/front/state_machine_def.hpp>
#include "Utils.hh"

namespace msm = boost::msm;

// The list of Occupancy_depth states

struct Idle : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Idle::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Waiting_for_ai_response : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Waiting_for_ai_response::" << __func__ << "]. " << reset << std::endl);
  }
};


#endif
