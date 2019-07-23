#define DEBUG

#ifndef _OCCUPANCY_DEPTH_STATES_
#define _OCCUPANCY_DEPTH_STATES_ 

//front-end
#include <boost/msm/front/state_machine_def.hpp>
#include "Utils.hh"

namespace msm = boost::msm;

// The list of Occupancy_depth states

struct None : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[None::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Extern : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Extern::" << __func__ << "]. " << reset << std::endl);
  }
};

//Internal state name gives me error
struct Intern : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Intern::" << __func__ << "]. " << reset << std::endl);
  }
};

struct Reserved : public msm::front::state<>{
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&){
    D(std::cout << info << "[Reserved::" << __func__ << "]. " << reset << std::endl);
  }
};

#endif
