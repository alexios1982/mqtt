#define DEBUG

#include "Alarm_system.hh"
#include <iostream>
#include "Utils.hh"

template <class Event, class FSM>
void Alarm_system_::on_entry(Event const&, FSM&){
  D(std::cout << info << "[Alarm_sytem_::" << __func__ << "]. "  << reset << std::endl);
}
