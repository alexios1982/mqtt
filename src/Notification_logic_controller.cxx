#define DEBUG

#include "Alarm_system_state_machine.hh"
#include <iostream>
#include "Utils.hh"
#include <boost/core/ignore_unused.hpp>

template <class Event>
void Notification_logic_controller::process_event_verbose(Event const& evt){
  D(std::cout << warning << __func__ << reset << std::endl);
  process_event(evt);
  // we have now several active states, which we show
  std::cerr << "statemachine states" << std::endl;
  for (unsigned int i = 0;
       i < Alarm_system::nr_regions::value;
       ++i){
    //D(std::cout << warning << "::print_current_states. Current states -> " << reset << states_name[current_state()[i]] << std::endl);
    std::cerr << "::print_current_states. Current states -> " << states_name[current_state()[i]] << std::endl;
  }  
}
